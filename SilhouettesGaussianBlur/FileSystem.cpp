#include <string>

#include "FileSystem.h"

using namespace std;

static Assimp::Importer importer;

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

const aiScene* FileSystem::loadModel(const string &name)
{
    auto path = string("../Resources/Models/" + name);

    const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene == NULL) cerr << "Can't Load " + path << endl;

    return pScene;
}

unsigned int FileSystem::getVertexCount(const aiMesh *mesh)
{
    return mesh->mNumVertices;
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