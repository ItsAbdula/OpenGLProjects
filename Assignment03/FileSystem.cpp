#include "FileSystem.h"

using namespace std;

string FileSystem::readFile(const string &path)
{
    ifstream inputStream;

    inputStream.open(path);
    if (inputStream.is_open() == false)
    {
        cerr << "Can't open : " << path << endl;
    }

    stringstream buffer;
    buffer << inputStream.rdbuf();

    auto contents(buffer.str());

    inputStream.close();

    return contents;
}

void FileSystem::writeFile(const string &path, const string &contents)
{
    ofstream outputStream;

    outputStream.open(path);
    if (outputStream.is_open() == false)
    {
        cerr << "Can't open : " << path << endl;
    }

    outputStream << contents << endl;

    outputStream.close();
}