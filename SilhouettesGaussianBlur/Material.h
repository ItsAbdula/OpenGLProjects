#pragma once

#include <glad/glad.h>

class Material
{
private:
    GLuint programID;

    GLuint diffuseMapID;
    GLuint specularMapID;

public:
    Material(GLuint _programID, GLuint _diffuseMap, GLuint _specularMap);

    GLuint getProgramID();

    GLuint getDiffuseMapID();
    GLuint getSpecularMapID();
};