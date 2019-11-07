#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <vector>

class Mesh
{
private:
    /* data */
    unsigned int _mesh[3];
public:
    Mesh(/* args */);
    ~Mesh();

    bool setMesh(const std::vector<float> & _mesh);

    void dcRender();
    void dcUpdate();
};




#endif