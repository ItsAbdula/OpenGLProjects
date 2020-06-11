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

    Transform *getTransform();
    Material *getMaterial();
    GLuint getVertexCount();

    void setMesh(Mesh& _mesh);
    void setMaterial(Material *_material);

    void render(Camera &camera);
    void ndotvRender(Camera & camera);
    void projectiveRender(Camera &camera, Camera &projector);
    void silhouetteRender(Camera &camera);
    void silhouetteGaussianBlurRender(Camera & camera);
};

GLint compileShader(const GLint shaderType, const std::string *shaderSource);
std::vector<GLint> compileShaders(const std::string *shaderSources);
float gauss(float x, float sigma2);
void setupQuad();
void setupFBO();
void setupGaussianBlurUniforms(GLuint programID);
GLuint buildProgram(const std::string name);

void setUniformValue(GLuint &prog, const char *name, glm::vec1 value);
void setUniformValue(GLuint &prog, const char *name, glm::ivec1 value);
void setUniformValue(GLuint &prog, const char *name, glm::vec2 &value);
void setUniformValue(GLuint &prog, const char *name, glm::ivec2 &value);
void setUniformValue(GLuint &prog, const char *name, glm::vec3 &value);
void setUniformValue(GLuint &prog, const char *name, glm::ivec3 &value);
void setUniformValue(GLuint &prog, const char *name, glm::vec4 &value);
void setUniformValue(GLuint &prog, const char *name, glm::ivec4 &value);
void setUniformValue(GLuint &prog, const char *name, glm::mat4 &value);
