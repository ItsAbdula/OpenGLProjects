#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class FileSystem
{
public:
    static bool isExist(const string &path);
    static string getExtension(const string &filePath);
    static string readFile(const string &path);
    static string readShader(const string &name);

    static void writeFile(const string &path, const string &contents);

private:
    FileSystem() {}
};
