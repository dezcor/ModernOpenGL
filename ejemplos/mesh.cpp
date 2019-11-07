#include "mesh.hpp"
// Incluir GLEW.
#include <GL/glew.h>

Mesh::Mesh(/* args */)
{
}

Mesh::~Mesh()
{
}

void Mesh::dcRender()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh[0]);
    glVertexAttribPointer(
        0,   // attribute
        3,   // number of elements per vertex
        GL_FLOAT,   // the type of each element
        GL_FALSE,   // take our values as-is         
        sizeof(float) * 3,  // offset of first element
        0                    //(GLvoid*) offsetof(Vertex3d, Vertex3d::color)  // offset
    );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    glDisableVertexAttribArray(0);
}
void Mesh::dcUpdate()
{
    
}

bool Mesh::setMesh(const std::vector<float> & _mesh)
{
    if(!_mesh.size()) return false;
    glGenBuffers(1, &this->_mesh[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, this->_mesh[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _mesh.size(), _mesh.data(), GL_STATIC_DRAW);
    
    return false;
}
