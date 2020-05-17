#pragma once

#include <iostream>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    const aiScene* loadModel(const std::string &name);
    void passaiscene(const aiScene* pScene);

private:
};
