#include "Material.h"

Material::Material(GLuint _programID, GLuint _diffuseMap, GLuint _specularMap)
{
    programID = _programID;

    diffuseMapID = _diffuseMap;
    specularMapID = _specularMap;
}

GLuint Material::getProgramID()
{
    return programID;
}

GLuint Material::getDiffuseMapID()
{
    return diffuseMapID;
}

GLuint Material::getSpecularMapID()
{
    return specularMapID;
}