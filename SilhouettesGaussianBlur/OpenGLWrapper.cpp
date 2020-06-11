#include "OpenGLWrapper.h"
#include "ShaderType.h"

// settings
const unsigned int _SCR_WIDTH = 800;
const unsigned int _SCR_HEIGHT = 600;

// lighting
glm::vec3 _lightPos(1.2f, 20.0f, 2.0f);

GLuint renderFBO = 0;
GLuint renderTex = 0;

GLuint intermediateFBO = 0;
GLuint intermediateTex = 0;

GLuint fsQuad = 0;

RenderObject::RenderObject()
{
    mesh = Mesh();
    transform = Transform();
}

RenderObject::RenderObject(Mesh& _mesh) : mesh(_mesh)
{
    transform = Transform();
}

Material *RenderObject::getMaterial()
{
    return material;
}

GLuint RenderObject::getVertexCount()
{
    return mesh.getVertexCount();
}

Transform *RenderObject::getTransform()
{
    return &transform;
}

void RenderObject::setMaterial(Material *_material)
{
    material = _material;
}

void RenderObject::render(Camera &camera)
{
    auto prog = material->getProgramID();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    setUniformValue(prog, "light.position", _lightPos);
    setUniformValue(prog, "viewPos", viewPos);

    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
    setUniformValue(prog, "light.ambient", ambient);
    setUniformValue(prog, "light.diffuse", diffuse);
    setUniformValue(prog, "light.specular", specular);

    setUniformValue(prog, "material.shininess", glm::fvec1{ 64.0f });

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseMapID());
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularMapID());
    }

    drawMesh(mesh);

    glUseProgram(0);
}

void RenderObject::ndotvRender(Camera &camera)
{
    auto prog = material->getProgramID();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    setUniformValue(prog, "viewPos", viewPos);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseMapID());
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularMapID());
    }

    drawMesh(mesh);

    glUseProgram(0);
}

void RenderObject::silhouetteRender(Camera &camera)
{
    auto prog = material->getProgramID();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    setUniformValue(prog, "viewPos", viewPos);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    setUniformValue(prog, "threshold", glm::fvec1(0.3f));

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseMapID());
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularMapID());
    }

    drawMesh(mesh);

    glUseProgram(0);
}

float gauss(float x, float sigma2)
{
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double expon = -(x * x) / (2.0 * sigma2);
    return (float)(coeff * exp(expon));
}

void setupQuad()
{
    // Array for full-screen quad
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // Set up the buffers

    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object

    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
    glEnableVertexAttribArray(2);  // Texture coordinates

    glBindVertexArray(0);
}

void setupFBO()
{
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

    // Create the texture object
    glGenTextures(1, &renderTex);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _SCR_WIDTH, _SCR_WIDTH);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _SCR_WIDTH, _SCR_HEIGHT);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate and bind the framebuffer
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // Create the texture object
    glGenTextures(1, &intermediateTex);
    glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, intermediateTex);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, _SCR_WIDTH, _SCR_HEIGHT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTex, 0);

    // No depth buffer needed for this FBO

    // Set the targets for the fragment output variables
    glDrawBuffers(1, drawBuffers);

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupGaussianBlurUniforms(GLuint programID)
{
    float weights[5], sum, sigma2 = 8.0f;

    // Compute and sum the weights
    weights[0] = gauss(0, sigma2);
    sum = weights[0];
    for (int i = 1; i < 5; i++)
    {
        weights[i] = gauss(float(i), sigma2);
        sum += 2 * weights[i];
    }

    // Normalize the weights and set the uniform
    for (int i = 0; i < 5; i++)
    {
        std::stringstream uniName;
        uniName << "Weight[" << i << "]";
        float val = weights[i] / sum;

        setUniformValue(programID, uniName.str().c_str(), glm::fvec1(val));
    }
}

void RenderObject::silhouetteGaussianBlurRender(Camera &camera)
{
    auto prog = material->getProgramID();
    glUseProgram(prog);

    ////pass1
    setUniformValue(prog, "Pass", glm::ivec1(1));

    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    auto viewPos = camera.transform.get_translate();
    setUniformValue(prog, "viewPos", viewPos);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    setUniformValue(prog, "threshold", glm::fvec1(0.3f));

    drawMesh(mesh);

    //pass2
    setUniformValue(prog, "Pass", glm::ivec1(2));

    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    ////pass3
    setUniformValue(prog, "Pass", glm::ivec1(3));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, intermediateTex);

    glClear(GL_COLOR_BUFFER_BIT);

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
}

void RenderObject::projectiveRender(Camera &camera, Camera &projector)
{
    glm::mat4 bias = { 0.5f, 0.0f, 0.0f, 0.5f,
                            0.0f, 0.5f, 0.0f, 0.5f,
                            0.0f, 0.0f, 0.5f, 0.5f,
                            0.0f, 0.0f, 0.0f, 1.0f };

    glm::mat4 projector_view = glm::lookAt(projector.transform.get_translate(), projector.transform.get_translate() + projector.transform.get_front(), projector.transform.get_up());
    glm::mat4 projector_projection = glm::perspective(glm::radians(projector.Zoom), 1.0f, 0.1f, 100.0f);

    auto prog = material->getProgramID();
    glUseProgram(prog);

    auto viewPos = camera.transform.get_translate();

    setUniformValue(prog, "light.position", _lightPos);
    setUniformValue(prog, "viewPos", viewPos);

    glm::vec3 a = { 0.2f, 0.2f, 0.2f };
    glm::vec3 d = { 0.5f, 0.5f, 0.5f };
    glm::vec3 f = { 1.0f, 1.0f, 1.0f };
    setUniformValue(prog, "light.ambient", a);
    setUniformValue(prog, "light.diffuse", d);
    setUniformValue(prog, "light.specular", f);

    setUniformValue(prog, "material.shininess", glm::fvec1{ 64.0f });

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_SCR_WIDTH / (float)_SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = transform.get_model_matrix();

    setUniformValue(prog, "projectorBias", bias);
    setUniformValue(prog, "projectorProjection", projector_projection);
    setUniformValue(prog, "projectorView", projector_view);

    setUniformValue(prog, "projection", projection);
    setUniformValue(prog, "view", view);
    setUniformValue(prog, "model", model);

    {
        setUniformValue(prog, "material.diffuse", glm::ivec1{ 0 });
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseMapID());
    }

    {
        setUniformValue(prog, "material.specular", glm::ivec1{ 1 });
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularMapID());
    }

    {
        setUniformValue(prog, "projImage", glm::ivec1{ 2 });
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->getSpecularMapID());
    }

    drawMesh(mesh);

    glUseProgram(0);
}

GLuint buildProgram(const std::string name)
{
    std::string shaderSources[5] = { "", "", "", "", "" };
    if (FileSystem::isExist("../Shaders/" + name + ".vert"))
    {
        shaderSources[0] = FileSystem::readShader("../Shaders/" + name + ".vert");
    }
    if (FileSystem::isExist("../Shaders/" + name + ".frag"))
    {
        shaderSources[1] = FileSystem::readShader("../Shaders/" + name + ".frag");
    }
    if (FileSystem::isExist("../Shaders/" + name + ".geom"))
    {
        shaderSources[2] = FileSystem::readShader("../Shaders/" + name + ".geom");
    }

    auto shaderIDs = compileShaders(shaderSources);

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

std::vector<GLint> compileShaders(const std::string *shaderSources)
{
    auto shaderIDs = std::vector<GLint>();
    if (shaderSources[0].compare("") == false)
    {
        shaderIDs.push_back(compileShader(GL_VERTEX_SHADER, &shaderSources[0]));
    }
    if (shaderSources[1].compare("") == false)
    {
        shaderIDs.push_back(compileShader(GL_FRAGMENT_SHADER, &shaderSources[1]));
    }
    if (shaderSources[2].compare("") == false)
    {
        shaderIDs.push_back(compileShader(GL_GEOMETRY_SHADER, &shaderSources[2]));
    }

    return shaderIDs;
}

GLint compileShader(const GLint shaderType, const std::string *shaderSource)
{
    GLuint shaderID;
    switch (shaderType)
    {
    case (GL_VERTEX_SHADER): shaderID = glCreateShader(GL_VERTEX_SHADER); break;
    case (GL_FRAGMENT_SHADER): shaderID = glCreateShader(GL_FRAGMENT_SHADER); break;
    case (GL_GEOMETRY_SHADER): shaderID = glCreateShader(GL_GEOMETRY_SHADER); break;
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

void setUniformValue(GLuint &prog, const char *name, glm::vec1 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform1fv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::ivec1 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform1iv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::vec2 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform2fv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::ivec2 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform2iv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::vec3 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform3fv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::ivec3 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform3iv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::vec4 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform4fv(uniform, 1, &value.x);
}
void setUniformValue(GLuint &prog, const char *name, glm::ivec4 value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniform4iv(uniform, 1, &value.x);
}

void setUniformValue(GLuint &prog, const char *name, glm::mat4 &value)
{
    auto uniform = glGetUniformLocation(prog, name);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
}