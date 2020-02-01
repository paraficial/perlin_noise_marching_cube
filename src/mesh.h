#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader;
class Mesh
{
public:
    Mesh();
    unsigned int addVertex(glm::vec3 vertex);
    unsigned int addNormal(glm::vec3 normal);
    void addTriangle(unsigned int i1, unsigned int i2, unsigned int i3);

    void gpu();
    void draw(Shader *shader);
    void exportOBJ(std::string path);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
private:
    GLuint vao;
    GLuint vbo_vertices, vbo_normals;
    GLuint ibo;
};

#endif // MESH_H
