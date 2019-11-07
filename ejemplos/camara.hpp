#ifndef _CAMARAGLFW_H_
#define _CAMARAGLFW_H_
// Incluir los cabeceros estándar
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iosfwd>
#include <sstream>
#include <fstream>
// Incluir GLEW.
#include <GL/glew.h>
// Incluir GLFW
#include <GLFW/glfw3.h>
// Incluir GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"

class Camara
{
  public:
    void run()
    {
        if (init() == -1)
            return;
        mainLoop();
        
        cleanup();
        
        glfwTerminate();
    }

    GLvoid rezise(GLuint W,GLuint H)
    {
        Width = W;
        Height = H;
    }

    std::string GetStrName()
    {
        return Name;
    }
    GLvoid SetStrName(const std::string &Name)
    {
        this->Name = Name;
    }
    // GLvoid SetStrName(const char* Name)
    // {
    //     this->Name = Name;
    // }
  protected:
    //Crear una ventana y su contexto OpenGL
    GLFWwindow *window;
    //point g_vertex_buffer_data[3];
    GLuint MatrixID;
    GLuint colorbuffer;
    GLuint Width = 600;
    GLuint Height = 600;

    GLuint program;
    GLuint vertexbuffer;
    GLuint VertexArrayID;

    std::string Name;
    
    bool isProjection = true;

    Mesh mesh;
    
    int init()
    {
        const int result = initGLWF();

        InitElementos();
        
        return result;
    }

    void mainLoop()
    {
        glViewport(0, 0, Width, Height);
        do
        {
            Update();
            draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        } // Revisar que la tecla ESC fue presionada y cerrar la ventana
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0);
    }

    virtual void cleanup()
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &VertexArrayID);
    }

    virtual void draw()
    {
        glUseProgram(program);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static float x = 0,y = 0,z = 0;
        glm::mat4 model;
        glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        model = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(z), glm::vec3(0, 0, 1));

        glm::mat4 GA_vertes =  tras * model;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(GA_vertes));
        mesh.dcRender();
        
        // if (y > 360)
        //     y = 0;
        // else
        //     y++;
        
        if (z > 360)
            z = 0;
        else
            z++;

    }

    virtual void Update()
    {
        
    }

    virtual void InitElementos()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        program = LoadShaders("vertex.shader.glsl","fragment.shader.glsl");

        MatrixID = glGetUniformLocation(program, "MVP");

        std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, // left 
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
        }; 
        mesh.setMesh(vertices);
    }

    GLint initGLWF()
    {
        // Inicializar GLFW
        if (!glfwInit())
        {
            fprintf(stderr, "Error al inicializar GLFW\n");
            return -1;
        }
        if(Name.empty())
            Name = "Window";
        glfwWindowHint(GLFW_SAMPLES, 2);               // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Queremos OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Para hacer feliz a MacOS ; Aunque no debería ser necesaria
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL
        
        window = glfwCreateWindow(this->Width, this->Height, Name.c_str(), NULL, NULL);
        if (window == NULL)
        {
            fprintf(stderr, "Falla al abrir una ventana GLFW. Si usted tiene una GPU Intel, está no es compatible con 3.3. Intente con la versión 2.1 de los tutoriales.\n");
            glfwTerminate();
            return -1;
        }
        
        glfwMakeContextCurrent(window); // Inicializar GLEW
        glewExperimental = true;        // Se necesita en el perfil de base.
        
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Falló al inicializar GLEW\n");
            return -1;
        }
        
        // Capturar la tecla ESC cuando sea presionada
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
        return 0;
    }

    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
    {

        // Crear los shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Leer el Vertex Shader desde archivo
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        
        if (VertexShaderStream.is_open())
        {
            std::stringstream sstr;
            sstr << VertexShaderStream.rdbuf();
            VertexShaderCode = sstr.str();
            VertexShaderStream.close();
        }
        else
        {
            fprintf(stderr,"Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        // Leer el Fragment Shader desde archivo
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        
        if (FragmentShaderStream.is_open())
        {
            std::stringstream sstr;
            sstr << FragmentShaderStream.rdbuf();
            FragmentShaderCode = sstr.str();
            FragmentShaderStream.close();
        }
        else
        {
            fprintf(stderr,"Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compilar Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const *VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Revisar Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }

        // Compilar Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const *FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Revisar Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }

        // Vincular el programa por medio del ID
        printf("Linking program\n");

        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Revisar el programa
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }

        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }
};

#endif //_CAMARAGLFW_H_