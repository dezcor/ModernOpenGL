# mordern OpenGL 

## Que es OpenGL
Es una especificación estándar que define una API multilenguaje y multiplataforma para escribir aplicaciones que produzcan gráficos 2D y 3D.
## Modern OpenGL
OpenGL ha evolucionado a lo largo de los años y se produjo un gran cambio en 2003 con la introducción de  dynamic pipeline (OpenGL 2.0), es decir, el uso de shaders que permiten tener acceso directo a la GPU.

Antes de esta versión, OpenGL usaba una pipeline fija y aún puede encontrar muchos tutoriales que todavía usan esta tubería fija. Esto introduce algunos cambios radicales en la forma de programar OpenGL y hace que sea más difícil de programar pero mucho más potente.

## Shaders
Los Shaders son piezas de programa que usan un lenguaje tipo C que se construyen en la GPU y se ejecutan durante la canalización de representación.

Dependiendo de la naturaleza de los shaders (hay muchos tipos dependiendo de la versión de OpenGL que esté usando), actuarán en diferentes etapas del rendering pipeline.

## Creacion de venta

### [GLFW](https://www.glfw.org/download.html)
GLFW es una biblioteca, escrita en C, específicamente dirigida a OpenGL que proporciona las necesidades básicas necesarias para mostrar los objetos en la pantalla. Nos permite crear un contexto OpenGL, definir parámetros de ventana y manejar la entrada del usuario, que es todo lo que necesitamos.
 
[Intalacion en Windows.](https://learnopengl.com/Getting-started/Creating-a-window)

### [GLEW](http://glew.sourceforge.net/)
OpenGL Extension Wrangler Library (GLEW) es una biblioteca de carga de extensiones C/C++ multiplataforma de código abierto. GLEW proporciona mecanismos eficientes de tiempo de ejecución para determinar qué extensiones de OpenGL son compatibles con la plataforma de destino. La funcionalidad de núcleo y extensión de OpenGL se expone en un solo archivo de encabezado. GLEW ha sido probado en una variedad de sistemas operativos, incluidos Windows, Linux, Mac OS X, FreeBSD, Irix y Solaris.

### Hola mundo en OpenGL
```
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialización y configuracion

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

    // glfw window creation

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew: load all OpenGL function pointers

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }    

    // render loop

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
```

Comando para compilacion.
```
g++ main.cpp -lglfw -lGL -lX11 -lGLEW -o main
```
