#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

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

Image *ResourceManager::makeImage(std::string fileName, int *width, int *height, int *nrChannels)
{
    fileName = "../Resources/Images/" + fileName;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(fileName.c_str(), width, height, nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture : " + fileName << std::endl;
    }

    Image *img = new Image(*width, *height, *nrChannels, data);

    return img;
}

void ResourceManager::unloadImage(Image *img)
{
    stbi_image_free(img->getData());
    free(img);
}

GLuint ResourceManager::loadImage(const std::string fileName, const ImageType type)
{
    int width, height, nrChannels;
    GLuint textureID;

    Image *tex = makeImage(fileName, &width, &height, &nrChannels);
    if (tex != NULL && tex->getData() != NULL)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        if (type == ImageType::REPEAT)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else if (type == ImageType::CLAMP)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, tex->getData());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    unloadImage(tex);

    return textureID;
}

Mesh *ResourceManager::makeMesh(MeshData *meshData)
{
    auto mesh = new Mesh(meshData);

    return mesh;
}