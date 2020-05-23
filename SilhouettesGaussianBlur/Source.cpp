#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

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
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
double decalX = SCR_WIDTH / 2.0f;
double decalY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;
double lastFrame = 0.0f;

// RenderObjects
std::map<std::string, RenderObject> RenderObjects;

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

    {
        glEnable(GL_DEPTH_TEST);
    }

    auto black = ResourceManager::getInstance().loadImage("black.png", ImageType::REPEAT);
    auto magenta = ResourceManager::getInstance().loadImage("magenta.png", ImageType::REPEAT);
    auto orange = ResourceManager::getInstance().loadImage("orange.png", ImageType::REPEAT);
    auto white = ResourceManager::getInstance().loadImage("white.png", ImageType::REPEAT);
    auto transparent = ResourceManager::getInstance().loadImage("transparent.png", ImageType::REPEAT);

    auto cow = Mesh(ResourceManager::getInstance().loadModel("spot_triangulated.obj"));
    auto cube = Mesh(ResourceManager::getInstance().loadModel("cube.obj"));
    auto plane = Mesh(ResourceManager::getInstance().loadModel("plane.obj"));
    auto sphere = Mesh(ResourceManager::getInstance().loadModel("sphere.obj"));
    auto teapot = Mesh(ResourceManager::getInstance().loadModel("teapot.obj"));

    auto renderObject = build_program("RenderObject");
    auto lighting = build_program("Lighting_Specular");
    auto lamp = build_program("Lighting_Lamp");
    auto lightmap = build_program("Lighting_Maps");
    auto textureProgram = build_program("Texture");

    auto defaultMaterial = new Material(lightmap, orange, transparent);

    RenderObjects["RenderCow"] = RenderObject(cow);
    {
        auto transform = RenderObjects["RenderCow"].get_transform();
        //transform->set_translate(glm::vec3(0.0f, -10.0f, -40.0f));
        //transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    }
    {
        RenderObjects["RenderCow"].set_material(defaultMaterial);
    }

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            RenderObjects["RenderCow"].render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deleteMesh(cow); // VRAM(GPU)

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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - lastX) / (height) * 180);
        double yoffset = ((lastY - ypos) / (width) * 180);
        lastX = xpos;
        lastY = ypos;

        //camera.ProcessMouseMovement(xoffset, yoffset);
        auto rotate = RenderObjects["RenderCow"].get_transform()->get_rotate();
        rotate.x -= yoffset;
        rotate.y += xoffset;

        RenderObjects["RenderCow"].get_transform()->set_rotate(rotate);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - decalX) / (height) * 180);
        double yoffset = ((decalY - ypos) / (width) * 180);
        decalX = xpos;
        decalY = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}