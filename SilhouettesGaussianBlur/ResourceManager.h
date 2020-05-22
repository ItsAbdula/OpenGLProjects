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
#include "Image.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    MeshData *loadModel(const std::string &name);
    Image * makeImage(std::string fileName, int * width, int * height, int * nrChannels);
    void unloadImage(Image * img);
    GLuint loadImage(const std::string fileName, const ImageType type);
    Mesh *makeMesh(MeshData *meshData);

private:
};
