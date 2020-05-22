#pragma once

#include <glad/glad.h>

class Material
{
private:
    GLuint programID;

    GLuint diffuseMap;
    GLuint specularMap;

public:
    Material(GLuint _programID, GLuint _diffuseMap, GLuint _specularMap);

    GLuint get_program();

    GLuint get_diffuseMap();
    GLuint get_specularMap();
};