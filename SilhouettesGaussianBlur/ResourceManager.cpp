#include "ResourceManager.h"

const aiScene* ResourceManager::loadModel(const std::string &name)
{
    auto path = std::string("../Resources/Models/" + name);

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (pScene == NULL)
    {
        std::cerr << importer.GetErrorString() << std::endl;

        return pScene;
    }

    passaiscene(pScene);

    return pScene;
}

void ResourceManager::passaiscene(const aiScene* pScene)
{
    if (pScene->HasMeshes())
    {
        for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
        {
            auto mesh = pScene->mMeshes[i];
            for (unsigned int j = 0; i < mesh->mNumVertices; j++)
            {
                auto vertex = mesh->mVertices[j];
            }
        }
    }
}