#pragma once
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "FileSystem.h"
#include "Mesh.h"
#include "Material.h"

class RenderObject
{
private:
    GLuint id;

    Transform transform;
    Mesh mesh;
    Material *material;

public:
    RenderObject();
    RenderObject(Mesh& _mesh);

    Transform *get_transform();
    Material *get_material();
    GLuint get_vertex_count();

    void setMesh(Mesh& _mesh);
    void set_material(Material *_material);

    void render(Camera &camera);
    void projective_render(Camera &camera, Camera &projector);
    void silhouetteRender(Camera &camera);
};

GLint compile_shader(const GLint shaderType, const std::string *shaderSource);
void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources);
GLuint build_program(const std::string name);

void set_uniform_value(GLuint &prog, const char *name, glm::vec1 value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec1 value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec2 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec2 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec3 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec3 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::vec4 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::ivec4 &value);
void set_uniform_value(GLuint &prog, const char *name, glm::mat4 &value);
