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

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;
double lastFrame = 0.0f;

// HowtoRender
int HowToRender = 1;

// Global Map
std::map<std::string, RenderObject> RenderObjects;
std::map<std::string, Material> Materials;

int main()
{
    string filePath = "";
    {
        std::cout << "Please enter the path of input file." << std::endl;
        std::cin >> filePath;
        if (FileSystem::isExist(filePath) == false)
        {
            std::cerr << "Can't find" + filePath << std::endl;
            return -1;
        }
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

    auto cow = Mesh(ResourceManager::getInstance().loadModelFromPath(filePath));

    auto ndotv = buildProgram("ndotv");
    auto silhouettesGeometry = buildProgram("SilhouettesGeometry");

    Materials["ndotv"] = Material(ndotv, 0, 0);
    Materials["SilhouettesGeometry"] = Material(silhouettesGeometry, 0, 0);

    RenderObjects["Cow"] = RenderObject(cow);
    {
        auto transform = RenderObjects["Cow"].getTransform();
        transform->setTransform(glm::vec3(0.0f, 0.0f, -5.0f));
        transform->setRotate(glm::vec3(0.0f, 180.0f, 0.0f));
    }
    {
        RenderObjects["Cow"].setMaterial(&Materials["SilhouettesGeometry"]);

        glUseProgram(Materials["SilhouettesGeometry"].getProgramID());

        setupSilhouettesGeometry(Materials["SilhouettesGeometry"].getProgramID());

        glUseProgram(0);
    }

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        {
            if (HowToRender == 1)
            {
                RenderObjects["Cow"].setMaterial(&Materials["ndotv"]);
                RenderObjects["Cow"].ndotvRender(camera);
            }
            else if (HowToRender == 2)
            {
                glUseProgram(Materials["SilhouettesGeometry"].getProgramID());

                auto progID = Materials["SilhouettesGeometry"].getProgramID();
                setUniformValue(progID, "onlySilhouette", glm::ivec1(0));

                glUseProgram(0);

                RenderObjects["Cow"].setMaterial(&Materials["SilhouettesGeometry"]);
                RenderObjects["Cow"].silhouetteGeometryRender(camera);
            }
            else if (HowToRender == 3)
            {
                glUseProgram(Materials["SilhouettesGeometry"].getProgramID());

                auto progID = Materials["SilhouettesGeometry"].getProgramID();
                setUniformValue(progID, "onlySilhouette", glm::ivec1(1));

                glUseProgram(0);

                RenderObjects["Cow"].setMaterial(&Materials["SilhouettesGeometry"]);
                RenderObjects["Cow"].silhouetteGeometryRender(camera);
            }
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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        HowToRender = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        HowToRender = 2;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        HowToRender = 3;
    }
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

        for (auto& elem : RenderObjects)
        {
            auto rotate = elem.second.getTransform()->getRotate();
            rotate.x -= yoffset;
            rotate.y += xoffset;

            elem.second.getTransform()->setRotate(rotate);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}