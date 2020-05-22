#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FileSystem.h"

class Shader
{
public:
    Shader(GLuint _ID) { ID = _ID; }
    GLuint getID() const { return ID; }

private:
    GLuint ID;
};

class Program
{
public:
    Program(GLuint _ID) { ID = _ID; }
    GLuint getID() const { return ID; }

private:
    GLuint ID;
};

class Factory
{
public:
    static Shader MakeShader(const GLint shaderType, const std::string *shaderSource)
    {
        GLuint shaderID;
        switch (shaderType)
        {
        case (GL_VERTEX_SHADER):
            shaderID = glCreateShader(GL_VERTEX_SHADER);
            break;

        case (GL_FRAGMENT_SHADER):
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
            break;

        default:
            //TODO : 쉐이더 타입 추가 및 에러 처리
            break;
        }

        const char *shaderSourceChar = (*shaderSource).c_str();
        glShaderSource(shaderID, 1, &shaderSourceChar, NULL);
        glCompileShader(shaderID);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return Shader(shaderID);
    }

    static Program *MakeProgram(std::vector<Shader> &shaders)
    {
        auto program = new Program(glCreateProgram());

        for (const auto &s : shaders)
        {
            glAttachShader(program->getID(), s.getID());
        }

        glLinkProgram(program->getID());

        for (const auto &s : shaders)
        {
            glDeleteShader(s.getID());
        }

        return program;
    }
};

class VertexShader : Shader {};
class FragmentShader : Shader {};