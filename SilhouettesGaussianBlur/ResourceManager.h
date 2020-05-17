#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Singleton.h"
#include "Mesh.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    Mesh *loadModel(const std::string &name);

private:
};
