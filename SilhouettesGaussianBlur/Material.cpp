#include "Material.h"

Material::Material(GLuint _programID, GLuint _diffuseMap, GLuint _specularMap)
{
    programID = _programID;

    diffuseMap = _diffuseMap;
    specularMap = _specularMap;
}

GLuint Material::get_program()
{
    return programID;
}

GLuint Material::get_diffuseMap()
{
    return diffuseMap;
}

GLuint Material::get_specularMap()
{
    return specularMap;
}