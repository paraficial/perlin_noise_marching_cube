#include "mesh.h"
#include <iostream>
#include "shader.h"

Mesh::Mesh()
{

}

unsigned int Mesh::addVertex(glm::vec3 vertex)
{
    unsigned int index = static_cast<unsigned int>(vertices.size());
    this->vertices.push_back(vertex);
    return index;
}

unsigned int Mesh::addNormal(glm::vec3 normal)
{
    unsigned int index = static_cast<unsigned int>(normals.size());
    this->normals.push_back(normal);
    return index;
}

void Mesh::addTriangle(unsigned int i1, unsigned int i2, unsigned int i3)
{
    this->indices.push_back(i1);
    this->indices.push_back(i2);
    this->indices.push_back(i3);
}

void Mesh::gpu()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &this->vbo_vertices);
    glGenBuffers(1, &this->vbo_normals);
    glGenBuffers(1, &this->ibo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->normals.size(), this->normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::draw(Shader *shader)
{
    shader->use();
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glPointSize(4.0f);
//    glDrawElements(GL_POINTS, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, nullptr);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Mesh::exportOBJ(std::string path) {
    std::ofstream objFile(path);
    for (unsigned int i = 0; i < vertices.size(); i++) {
        char line[1024];
        sprintf(line, "v %f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
        objFile << line;
    }
    for (unsigned int i = 0; i < normals.size(); i++) {
        char line[1024];
        sprintf(line, "vn %f %f %f\n", normals[i].x, normals[i].y, normals[i].z);
        objFile << line;
    }
    for (unsigned int i = 0; i < indices.size(); i+=3) {
        char line[1024];
        sprintf(line, "f %u//%u %u//%u %u//%u\n", indices[i]+1, indices[i]+1, indices[i+1]+1, indices[i+1]+1, indices[i+2]+1, indices[i+2]+1);
        objFile << line;
    }
    objFile.close();
}






