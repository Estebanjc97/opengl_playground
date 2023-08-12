#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::Init() 
{
	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed.");
		glfwTerminate();
		return 1;
	}

	//Setup glfw window properties
	//Setting the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Make sure we are usen long term support code / Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatibility

	mainWindow = glfwCreateWindow(width, height, "Open GL", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);


	//Set context for GLEW to use / I will tie all that is gonna be drawn is gonna be in the mainWindow
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension feature  
	glewExperimental = GL_TRUE;

	//Initializing GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}