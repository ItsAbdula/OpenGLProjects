#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

using std::vector;

class MeshData
{
public:
    MeshData(const aiScene* scene);

    const GLuint countVertices();
    const GLuint countFaces();
    const GLuint countIndices();

    vector<glm::vec3> *getVertices();
    vector<GLuint>& getIndices();
    glm::vec3 *getVertex(GLuint vertexIndex);
    vector<GLuint> *getFace(GLuint faceIndex);
    vector<const glm::vec3*> getFaceVertices(GLuint faceIndex);

private:
    vector<glm::vec3> vertices;
    vector<vector<GLuint>> faces;
    vector<GLuint> indices;

    vector<glm::vec3> copyVertices(const aiScene* pScene);
    vector<vector<GLuint>> copyFaces(const aiScene* pScene);
    vector<GLuint> copyIndices(const aiScene* pScene);
};

class Mesh
{
public:
    //Mesh(GLuint _nVertex, GLuint _VAO, GLuint *_VBOs);
    Mesh(MeshData *_meshData);

    const GLuint get_vertex_count();
    const GLuint get_index_count();
    const GLuint getVAO();
    const vector<GLuint> getVBOs();
    const GLuint getEBO();

private:
    GLuint nVertices;
    GLuint nFaces;
    GLuint nIndices;

    GLuint VAO;
    vector<GLuint> VBOs;
    GLuint EBO;
};
GLuint allocateVBO(const GLuint attribIndex, vector<glm::vec3> *VBO);
GLuint allocateVBO(const GLuint attribIndex, vector<glm::vec2> *VBO);
GLuint *allocateVBOs(GLuint VAO, vector<vector<glm::vec3> *> &vertexInfoVec3, vector<vector<glm::vec2> *> &vertexInfoVec2);

GLuint allocateVAO();

GLuint allocateEBO(vector<GLuint> indices);

void drawMesh(Mesh &mesh);
void deleteMesh(Mesh &mesh);
