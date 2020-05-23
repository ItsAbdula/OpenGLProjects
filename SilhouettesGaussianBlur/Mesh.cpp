#include "Mesh.h"

Mesh::Mesh(MeshData *_meshData)
{
    nVertices = _meshData->countVertices();
    nFaces = _meshData->countFaces();
    nIndices = _meshData->countIndices();

    VAO = allocateVAO();
    {
        glBindVertexArray(VAO);
        VBOs.push_back(allocateVBO(_meshData->getVertexData()));
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO);
    EBO = allocateEBO(_meshData->getIndices());
    glBindVertexArray(0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const GLuint Mesh::get_vertex_count()
{
    return nVertices;
}

const GLuint Mesh::get_index_count()
{
    return nIndices;
}

const GLuint Mesh::getVAO()
{
    return VAO;
}

const vector<GLuint> Mesh::getVBOs()
{
    return VBOs;
}

const GLuint Mesh::getEBO()
{
    return EBO;
}

GLuint allocateVAO()
{
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);

    return VAO;
}

GLuint allocateEBO(vector<GLuint> indices)
{
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    return EBO;
}

GLuint allocateVBO(vector<VertexData>& vertexData)
{
    GLuint VBOIndex = 0;
    glGenBuffers(1, &VBOIndex);

    glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &(vertexData.front()), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoord));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VBOIndex;
}

GLuint allocateVBO(const GLuint attribIndex, vector<glm::vec3>& vertexData)
{
    GLuint VBOIndex = 0;
    glGenBuffers(1, &VBOIndex);

    glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &(vertexData.front()), GL_STATIC_DRAW);

    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VBOIndex;
}

void drawMesh(Mesh &mesh)
{
    glBindVertexArray(mesh.getVAO());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getEBO());
    glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void deleteMesh(Mesh &mesh)
{
    auto VAO = mesh.getVAO();
    auto VBOs = mesh.getVBOs();
    auto EBO = mesh.getEBO();

    glDeleteVertexArrays(1, &VAO);

    if (mesh.getVBOs().size() != 0)
    {
        glDeleteBuffers(VBOs.size(), &(VBOs.at(0)));
    }
    glDeleteBuffers(1, &EBO);
}

MeshData::MeshData(const aiScene* _scene)
{
    if (_scene == NULL) return;
    if (_scene->HasMeshes() == false) return;

    vertexData = copyVertexData(_scene);

    vertexPositions = copyVertexPositions(_scene);
    vertexNormals = copyVertexNormals(_scene);
    vertexTextureCoords = copyVertexTextureCoords(_scene);

    faces = copyFaces(_scene);
    indices = copyIndices(_scene);
}

const GLuint MeshData::countVertices()
{
    return vertexPositions.size();
}

const GLuint MeshData::countFaces()
{
    return faces.size();
}

const GLuint MeshData::countIndices()
{
    return indices.size();
}

vector<VertexData>& MeshData::getVertexData()
{
    return vertexData;
}

vector<VertexData> MeshData::copyVertexData(const aiScene* pScene)
{
    vector<VertexData> vertexData;
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];

        VertexData vertexDatum;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            vertexDatum.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
            vertexDatum.normal = glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
            vertexDatum.texCoord = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);

            vertexData.push_back(vertexDatum);
        }
    }

    return vertexData;
}

vector<glm::vec3> MeshData::copyVertexPositions(const aiScene* pScene)
{
    vector<glm::vec3> vertices;
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

vector<glm::vec3> MeshData::copyVertexNormals(const aiScene * pScene)
{
    vector<glm::vec3> normals;
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        if (mesh->HasNormals() == false) return normals;

        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            normals.push_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
        }
    }

    return normals;
}

vector<glm::vec2> MeshData::copyVertexTextureCoords(const aiScene * pScene)
{
    vector<glm::vec2> texCoords;
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        if (mesh->HasTextureCoords(0) == false) return texCoords;

        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y));
        }
    }

    return texCoords;
}

vector<vector<GLuint>> MeshData::copyFaces(const aiScene* pScene)
{
    vector<vector<GLuint>> faces;
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            auto face = mesh->mFaces[j];

            vector<GLuint> faceIndicies;
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                faceIndicies.push_back(face.mIndices[k]);
            }

            faces.push_back(faceIndicies);
        }
    }

    return faces;
}

vector<GLuint> MeshData::copyIndices(const aiScene* pScene)
{
    vector<GLuint> indices;
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        auto mesh = pScene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            auto face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    return indices;
}

glm::vec3 *MeshData::getVertexPosition(GLuint vertexIndex)
{
    return &(vertexPositions.at(vertexIndex));
}

vector<glm::vec3>& MeshData::getVertexPositions()
{
    return vertexPositions;
}

vector<glm::vec3>& MeshData::getVertexNormals()
{
    return vertexNormals;
}

vector<glm::vec2>& MeshData::getVertexTextureCoords()
{
    return vertexTextureCoords;
}

vector<GLuint>& MeshData::getIndices()
{
    return indices;
}

vector<GLuint>* MeshData::getFace(GLuint faceIndex)
{
    return &(faces.at(faceIndex));
}

vector<const glm::vec3*> MeshData::getFaceVertices(GLuint faceIndex)
{
    vector<const glm::vec3*> result;
    for (auto vertexIndex : *getFace(faceIndex))
    {
        result.push_back(getVertexPosition(vertexIndex));
    }

    return result;
}