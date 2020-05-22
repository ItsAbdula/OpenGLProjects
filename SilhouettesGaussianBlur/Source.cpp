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

	auto obj = Mesh(ResourceManager::getInstance().loadModel("spot_triangulated.obj"));

	auto vertexShader = Factory::MakeShader(GL_VERTEX_SHADER, &FileSystem::readShader("simpleShader.vert"));
	auto fragmentShader = Factory::MakeShader(GL_FRAGMENT_SHADER, &FileSystem::readShader("simpleShader.frag"));

	std::vector<Shader> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	auto program = Factory::MakeProgram(shaders);

	auto cam_programID = build_program("RenderObject");

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.785f, 0.601f, 0.242f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(obj.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, obj.get_vertex_count());
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	deleteMesh(obj);

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
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}