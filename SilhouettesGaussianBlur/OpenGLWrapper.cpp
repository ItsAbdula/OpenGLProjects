#include "OpenGLWrapper.h"
#include "ShaderType.h"

// settings
const unsigned int _SCR_WIDTH = 800;
const unsigned int _SCR_HEIGHT = 600;

// lighting
glm::vec3 _lightPos(1.2f, 20.0f, 2.0f);

Material::Material(GLuint _prog, GLuint _diffuseMap, GLuint _specularMap)
{
    prog = _prog;

    diffuseMap = _diffuseMap;
    specularMap = _specularMap;
}

GLuint Material::get_program()
{
    return prog;
}
GLuint Material::get_diffuseMap()
{
    return diffuseMap;
}
GLuint Material::get_specularMap()
{
    return specularMap;
}

RenderedObject::RenderedObject(Mesh * _mesh)
{
    transform = Transform();
    mesh = _mesh;
}

Material *RenderedObject::get_material()
{
    return material;
}

GLuint RenderedObject::get_vertex_count()
{
    return mesh->get_vertex_count();
}

Transform *RenderedObject::get_transform()
{
    return &transform;
}

void RenderedObject::set_material(Material *_material)
{
    material = _material;
}

void RenderedObject::render(Camera &camera)
{
    auto prog = material->get_program();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    set_uniform_value(prog, "light.position", _lightPos);
    set_uniform_value(prog, "viewPos", viewPos);

    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
    set_uniform_value(prog, "light.ambient", ambient);
    set_uniform_value(prog, "light.diffuse", diffuse);
    set_uniform_value(prog, "light.specular", specular);

    set_uniform_value(prog, "material.shininess", glm::fvec1{ 64.0f });

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    set_uniform_value(prog, "projection", projection);
    set_uniform_value(prog, "view", view);
    set_uniform_value(prog, "model", model);

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->get_diffuseMap());
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
    }

    drawMesh(*mesh);

    glUseProgram(0);
}

void RenderedObject::projective_render(Camera &camera, Camera &projector)
{
    glm::mat4 bias = { 0.5f, 0.0f, 0.0f, 0.5f,
                            0.0f, 0.5f, 0.0f, 0.5f,
                            0.0f, 0.0f, 0.5f, 0.5f,
                            0.0f, 0.0f, 0.0f, 1.0f };

    glm::mat4 projector_view = glm::lookAt(projector.transform.get_translate(), projector.transform.get_translate() + projector.transform.get_front(), projector.transform.get_up());
    glm::mat4 projector_projection = glm::perspective(glm::radians(projector.Zoom), 1.0f, 0.1f, 100.0f);

    auto prog = material->get_program();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    set_uniform_value(prog, "light.position", _lightPos);
    set_uniform_value(prog, "viewPos", viewPos);

    glm::vec3 a = { 0.2f, 0.2f, 0.2f };
    glm::vec3 d = { 0.5f, 0.5f, 0.5f };
    glm::vec3 f = { 1.0f, 1.0f, 1.0f };
    set_uniform_value(prog, "light.ambient", a);
    set_uniform_value(prog, "light.diffuse", d);
    set_uniform_value(prog, "light.specular", f);

    set_uniform_value(prog, "material.shininess", glm::fvec1{ 64.0f });

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    set_uniform_value(prog, "projectorBias", bias);
    set_uniform_value(prog, "projectorProjection", projector_projection);
    set_uniform_value(prog, "projectorView", projector_view);

    set_uniform_value(prog, "projection", projection);
    set_uniform_value(prog, "view", view);
    set_uniform_value(prog, "model", model);

    {
        set_uniform_value(prog, "material.diffuse", glm::ivec1{ 0 });
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->get_diffuseMap());
    }

    {
        set_uniform_value(prog, "material.specular", glm::ivec1{ 1 });
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
    }

    {
        set_uniform_value(prog, "projImage", glm::ivec1{ 2 });
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->get_specularMap());
    }

    drawMesh(*mesh);

    glUseProgram(0);
}

RenderedObject *make_render_object(Mesh *mesh)
{
    RenderedObject *ro = new RenderedObject(mesh);

    return ro;
}

GLuint build_program(const std::string name)
{
    std::string shaderSources[5];
    shaderSources[0] = FileSystem::readShader("../Shaders/" + name + ".vert");
    shaderSources[1] = FileSystem::readShader("../Shaders/" + name + ".frag");

    std::vector<GLint> shaderIDs;
    compile_shaders(&shaderIDs, shaderSources);

    auto programID = glCreateProgram();
    for (const auto &i : shaderIDs)
    {
        glAttachShader(programID, i);
    }
    glLinkProgram(programID);
    for (const auto &i : shaderIDs)
    {
        glDeleteShader(i);
    }

    return programID;
}

void compile_shaders(std::vector<GLint> *shaderIDs, const std::string *shaderSources)
{
    shaderIDs->push_back(compile_shader(GL_VERTEX_SHADER, &shaderSources[0]));
    shaderIDs->push_back(compile_shader(GL_FRAGMENT_SHADER, &shaderSources[1]));
}

GLint compile_shader(const GLint shaderType, const std::string *shaderSource)
{
    GLuint shaderID;
    switch (shaderType)
    {
    case (GL_VERTEX_SHADER): shaderID = glCreateShader(GL_VERTEX_SHADER); break;
    case (GL_FRAGMENT_SHADER): shaderID = glCreateShader(GL_FRAGMENT_SHADER); break;
    default:
        GLchar infoLog[512];
        std::cerr << infoLog << "SHADER::CAN'T_FIND_SHADER_TYPE" << std::endl;

        break;
    }

    const char *c_str = (*shaderSource).c_str();
    glShaderSource(shaderID, 1, &c_str, NULL);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << infoLog << "SHADER::" + parse_shaderType(shaderType) + "::COMPILATION_ERROR" << std::endl;
    }

    return shaderID;
}

GLint link_program(const GLint *shaderIDs)
{
    auto programID = glCreateProgram();
    glAttachShader(programID, shaderIDs[0]);
    glAttachShader(programID, shaderIDs[1]);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        std::cerr << infoLog << "PROGRAM::LINK_FAILED" << std::endl;
    }

    glDeleteShader(shaderIDs[0]);
    glDeleteShader(shaderIDs[1]);

    return programID;
}

void set_uniform_value(GLuint &prog, const char *name, glm::vec1 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform1fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec1 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform1iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec2 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform2fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec2 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform2iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec3 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform3fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec3 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform3iv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::vec4 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform4fv(uniform, 1, &value.x);
}
void set_uniform_value(GLuint &prog, const char *name, glm::ivec4 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform4iv(uniform, 1, &value.x);
}

void set_uniform_value(GLuint &prog, const char *name, glm::mat4 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
}