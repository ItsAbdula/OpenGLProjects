#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

class Mesh
{
public:
    Mesh(const aiScene* scene);

    const GLuint countVertices();
    const GLuint countFaces();

    const glm::vec3 *getVertex(GLuint vertexIndex);
    const std::vector<GLuint> *getFace(GLuint faceIndex);
    std::vector<const glm::vec3*> getFaceVertices(GLuint faceIndex);

private:
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<GLuint>> faces;

    std::vector<glm::vec3> copyVertices(const aiScene* pScene);
    std::vector<std::vector<GLuint>> copyFaces(const aiScene* pScene);
};
