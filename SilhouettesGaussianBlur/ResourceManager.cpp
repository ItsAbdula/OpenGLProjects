#include "ResourceManager.h"

Mesh *ResourceManager::loadModel(const std::string &name)
{
    auto path = std::string("../Resources/Models/" + name);

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (pScene == NULL)
    {
        std::cerr << importer.GetErrorString() << std::endl;
    }

    auto mesh = new Mesh(pScene);

    return mesh;
}