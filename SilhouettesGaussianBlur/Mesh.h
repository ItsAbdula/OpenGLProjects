#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

using std::vector;

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class MeshData
{
public:
    MeshData(const aiScene* scene);

    const GLuint countVertices();
    const GLuint countFaces();
    const GLuint countIndices();

    vector<VertexData>& getVertexData();

    vector<glm::vec3>& getVertexPositions();
    vector<glm::vec3>& getVertexNormals();
    vector<glm::vec2>& getVertexTextureCoords();
    vector<GLuint>& getIndices();
    glm::vec3 *getVertexPosition(GLuint vertexIndex);
    vector<GLuint> *getFace(GLuint faceIndex);
    vector<const glm::vec3*> getFaceVertices(GLuint faceIndex);

private:
    vector<VertexData> vertexData;

    vector<glm::vec3> vertexPositions;
    vector<glm::vec3> vertexNormals;
    vector<glm::vec2> vertexTextureCoords;
    vector<vector<GLuint>> faces;
    vector<GLuint> indices;

    vector<VertexData> copyVertexData(const aiScene * pScene);

    vector<glm::vec3> copyVertexPositions(const aiScene* pScene);
    vector<glm::vec3> copyVertexNormals(const aiScene* pScene);
    vector<glm::vec2> copyVertexTextureCoords(const aiScene* pScene);
    vector<vector<GLuint>> copyFaces(const aiScene* pScene);
    vector<GLuint> copyIndices(const aiScene* pScene);
};

class Mesh
{
public:
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
GLuint allocateVBO(const GLuint attribIndex, vector<glm::vec3>& vertexData);
GLuint allocateVBO(vector<VertexData>& vertexData);
GLuint allocateVAO();
GLuint allocateEBO(vector<GLuint> indices);

void drawMesh(Mesh &mesh);
void deleteMesh(Mesh &mesh);
