#include "ResourceManager.h"

MeshData *ResourceManager::loadModel(const std::string &name)
{
    auto path = std::string("../Resources/Models/" + name);

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (pScene == NULL)
    {
        std::cerr << importer.GetErrorString() << std::endl;
    }

    auto mesh = new MeshData(pScene);

    return mesh;
}

Mesh *ResourceManager::makeMesh(MeshData *meshData)
{
    auto mesh = new Mesh(meshData);

    return mesh;
}