#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

class MeshData
{
public:
    MeshData(const aiScene* scene);

    const GLuint countVertices();
    const GLuint countFaces();

    std::vector<glm::vec3> *getVertices();
    glm::vec3 *getVertex(GLuint vertexIndex);
    std::vector<GLuint> *getFace(GLuint faceIndex);
    std::vector<const glm::vec3*> getFaceVertices(GLuint faceIndex);

private:
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<GLuint>> faces;

    std::vector<glm::vec3> copyVertices(const aiScene* pScene);
    std::vector<std::vector<GLuint>> copyFaces(const aiScene* pScene);
};

class Mesh
{
public:
    //Mesh(GLuint _nVertex, GLuint _VAO, GLuint *_VBOs);
    Mesh(MeshData *_meshData);

    GLuint get_vertex_count();
    GLuint getVAO();
    const std::vector<GLuint> getVBOs();

private:
    GLuint nVertices;
    GLuint nFaces;

    GLuint VAO;
    std::vector<GLuint> VBOs;
};
GLuint allocateVBO(const GLuint attribIndex, std::vector<glm::vec3> *VBO);
GLuint allocateVBO(const GLuint attribIndex, std::vector<glm::vec2> *VBO);
GLuint *allocateVBOs(GLuint VAO, std::vector<std::vector<glm::vec3> *> &vertexInfoVec3, std::vector<std::vector<glm::vec2> *> &vertexInfoVec2);

GLuint allocateVAO();

void drawMesh(Mesh &mesh);
void deleteMesh(Mesh &mesh);
