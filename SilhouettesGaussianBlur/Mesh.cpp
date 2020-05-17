#include "Mesh.h"

Mesh::Mesh(const aiScene* _scene)
{
    if (_scene == NULL) return;

    vertices = copyVertices(_scene);
    faces = copyFaces(_scene);
}

std::vector<glm::vec3> Mesh::copyVertices(const aiScene* pScene)
{
    std::vector<glm::vec3> vertices;
    if (pScene->HasMeshes() == false) return vertices;

    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            vertices.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
        }
    }

    return vertices;
}

std::vector<std::vector<GLuint>> Mesh::copyFaces(const aiScene* pScene)
{
    std::vector<std::vector<GLuint>> faces;
    if (pScene->HasMeshes() == false) return faces;

    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            auto face = mesh->mFaces[j];

            std::vector<GLuint> faceIndicies;
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                faceIndicies.push_back(face.mIndices[k]);
            }

            faces.push_back(faceIndicies);
        }
    }

    return faces;
}
const glm::vec3 *Mesh::getVertex(GLuint vertexIndex)
{
    return &(vertices.at(vertexIndex));
}

const std::vector<GLuint>* Mesh::getFace(GLuint faceIndex)
{
    return &(faces.at(faceIndex));
}

std::vector<const glm::vec3*> Mesh::getFaceVertices(GLuint faceIndex)
{
    std::vector<const glm::vec3*> result;
    for (auto vertexIndex : *getFace(faceIndex))
    {
        result.push_back(getVertex(vertexIndex));
    }

    return result;
}