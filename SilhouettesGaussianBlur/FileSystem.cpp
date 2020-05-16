#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "FileSystem.h"

using namespace std;

string FileSystem::readFile(const string &path)
{
    string contents;

    ifstream fileStream(path);
    if (fileStream.is_open() == false)
    {
        cerr << "Can't open : " << path << endl;

        return contents;
    }

    stringstream buffer;
    buffer << fileStream.rdbuf();
    contents = buffer.str();

    fileStream.close();

    return contents;
}

string FileSystem::readShader(const string &name)
{
    return readFile("../Shaders/" + name);
}

void FileSystem::loadModel(const string &name)
{
    auto path = string("../Resources/Models/" + name);

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene == NULL) cerr << "Can't Load " + path << endl;
}

void FileSystem::writeFile(const string &path, const string &contents)
{
    ofstream outputStream(path);
    if (outputStream.is_open() == false)
    {
        cerr << "Can't open : " << path << endl;

        return;
    }

    outputStream << contents << endl;

    outputStream.close();
}