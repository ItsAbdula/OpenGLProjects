#include "Mesh.h"

Mesh::Mesh(MeshData *_meshData)
{
    nVertices = _meshData->countVertices();
    nFaces = _meshData->countFaces();

    VAO = allocateVAO();
    {
        allocateVBO(0, _meshData->getVertex[0]);
    }
}

GLuint Mesh::get_vertex_count()
{
    return nVertices;
}
GLuint Mesh::getVAO()
{
    return VAO;
}
std::vector<GLuint> Mesh::getVBOs()
{
    return VBOs;
}

GLuint allocateVAO()
{
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);

    return VAO;
}

GLuint allocateVBO(const GLuint attribIndex, std::vector<glm::vec3> *VBO)
{
    GLuint VBOIndex = 0;
    glGenBuffers(1, &VBOIndex);

    glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
    glBufferData(GL_ARRAY_BUFFER, VBO->size() * sizeof(glm::vec3), &(VBO->front()), GL_STATIC_DRAW);

    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GL_ZERO);

    return VBOIndex;
}

GLuint allocateVBO(const GLuint attribIndex, std::vector<glm::vec2> *VBO)
{
    GLuint VBOIndex = 0;
    glGenBuffers(1, &VBOIndex);

    glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
    glBufferData(GL_ARRAY_BUFFER, VBO->size() * sizeof(glm::vec2), &(VBO->front()), GL_STATIC_DRAW);

    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VBOIndex;
}

GLuint *allocateVBOs(GLuint VAO, std::vector<std::vector<glm::vec3> *> &vertexInfoVec3, std::vector<std::vector<glm::vec2> *> &vertexInfoVec2)
{
    GLuint *VBOindicies = new GLuint[vertexInfoVec3.size() + vertexInfoVec2.size()];

    glBindVertexArray(VAO);

    GLuint i = 0;
    for (i = 0; i < vertexInfoVec3.size(); i++)
    {
        VBOindicies[i] = allocateVBO(i, vertexInfoVec3.at(i));
    }
    for (i = 0; i < vertexInfoVec2.size(); i++)
    {
        VBOindicies[i] = allocateVBO(i, vertexInfoVec2.at(i));
    }

    glBindVertexArray(0);

    return VBOindicies;
}

void drawMesh(Mesh &mesh)
{
    glBindVertexArray(mesh.getVAO());

    glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());

    glBindVertexArray(0);
}

MeshData::MeshData(const aiScene* _scene)
{
    if (_scene == NULL) return;

    vertices = copyVertices(_scene);
    faces = copyFaces(_scene);
}

const GLuint MeshData::countVertices()
{
    return vertices.size();
}

const GLuint MeshData::countFaces()
{
    return faces.size();
}

std::vector<glm::vec3> MeshData::copyVertices(const aiScene* pScene)
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

std::vector<std::vector<GLuint>> MeshData::copyFaces(const aiScene* pScene)
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
const glm::vec3 *MeshData::getVertex(GLuint vertexIndex)
{
    return &(vertices.at(vertexIndex));
}

const std::vector<GLuint>* MeshData::getFace(GLuint faceIndex)
{
    return &(faces.at(faceIndex));
}

std::vector<const glm::vec3*> MeshData::getFaceVertices(GLuint faceIndex)
{
    std::vector<const glm::vec3*> result;
    for (auto vertexIndex : *getFace(faceIndex))
    {
        result.push_back(getVertex(vertexIndex));
    }

    return result;
}