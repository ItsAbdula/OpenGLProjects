#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "FileSystem.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Shader.h"

#include "OpenGLWrapper.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_btn_callBack(GLFWwindow* window, int btn, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
Camera projector(glm::vec3(0.0f, 0.0f, 50.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
double decalX = SCR_WIDTH / 2.0f;
double decalY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;
double lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "201421062_ChoiWooHyung", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto obj = ResourceManager::getInstance().loadModel("spot_triangulated.obj");

    auto vertexShaderSource = FileSystem::readShader("simpleShader.vert");
    auto vertexShader = Factory::MakeShader(GL_VERTEX_SHADER, &vertexShaderSource);

    auto fragmentShaderSource = FileSystem::readShader("simpleShader.frag");
    auto fragmentShader = Factory::MakeShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);

    std::vector<Shader> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    auto program = Factory::MakeProgram(shaders);

    float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
    };
    float colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,// top right
    1.0f, 0.0f, 0.0f, 1.0f,// bottom right
    0.0f, 0.0f, 1.0f, 1.0f,// bottom left
    0.0f, 0.0f, 1.0f, 1.0f // top left
    };
    unsigned int indices[] = { // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, obj->countVertices() * sizeof(glm::vec3), obj->getVertex(0), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->countFaces() * sizeof(GLuint), obj->getFace(0), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(program->getID());

    auto cam_programID = build_program("Camera");

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        /*
        201421062 -> {201, 421, 062} -> {201, 164, 062} -> {0.785, 0.601, 0.242}

        */
        glClearColor(0.785f, 0.601f, 0.242f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = camera.transform.get_model_matrix();

            set_uniform_value(cam_programID, "projection", projection);
            set_uniform_value(cam_programID, "view", view);
            set_uniform_value(cam_programID, "model", model);
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj->countVertices());
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_btn_callBack(GLFWwindow* window, int btn, int action, int mods)
{
    if (btn == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &lastX, &lastY);
    }

    if (btn == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &decalX, &decalX);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // left click and drag: move camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - lastX) / (height) * 180);
        double yoffset = ((lastY - ypos) / (width) * 180);
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // right click and drag: move projector image
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - decalX) / (height) * 180);
        double yoffset = ((decalY - ypos) / (width) * 180);
        decalX = xpos;
        decalY = ypos;
        projector.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}