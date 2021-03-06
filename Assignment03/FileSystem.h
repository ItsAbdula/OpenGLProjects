#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class FileSystem
{
public:
    static string readFile(const string &path);
    static void writeFile(const string &path, const string &contents);

private:
    FileSystem() {}
};