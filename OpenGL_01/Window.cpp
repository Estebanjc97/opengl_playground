#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0;
	bufferHeight = 0;
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0;
	bufferHeight = 0;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	for (size_t i = 0; i < 2; i++)
	{
		mouseButtons[i] = 0;
	}
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
	//glfwMaximizeWindow(mainWindow);
	
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

	CreateCallbacks();
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide the cursor

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

	//Configuracion para un viewport del 80% de la ventana y centrarlo
	glfwGetWindowSize(mainWindow, &width, &height);

	int openglViewportWidth = static_cast<int>(width * 1);
	int openglViewportHeight = static_cast<int>(height * 1);

	glViewport((width - openglViewportWidth) / 2, (height - openglViewportHeight) / 2, openglViewportWidth, openglViewportHeight);



	glfwSetWindowUserPointer(mainWindow, this); //Esto es como una subscripcion de esta clase a los eventos que ocurren en el mainWindow
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
	glfwSetMouseButtonCallback(mainWindow, HandleMouseButtons);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* _window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			_window->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			_window->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void Window::HandleMouseButtons(GLFWwindow* window, int button, int action, int mode)
{
	Window* _window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if ( button >= 0 && button < 3 && action == GLFW_PRESS)
	{
		_window->mouseButtons[button] = true;
	}
	if (button >= 0 && button < 3 && action == GLFW_RELEASE)
	{
		_window->mouseButtons[button] = false;
	}
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* _window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (_window->mouseFirstMoved)
	{
		_window->lastX = xPos;
		_window->lastY = yPos;
		_window->mouseFirstMoved = false;
	}

	_window->xChange = xPos - _window->lastX;
	_window->yChange = _window->lastY - yPos;

	_window->lastX = xPos;
	_window->lastY = yPos;

	/*printf("Actual Pixel position -> x:%.2f , y:%.2f \n", xPos, yPos);
	printf("Delta				  -> x:%.2f , y:%.2f \n", _window->xChange, _window->yChange);*/
}


//This can be handled better
GLfloat Window::GetXChange()
{
	GLfloat _change = xChange;
	xChange = 0.0f;
	return _change;
}

GLfloat Window::GetYChange()
{
	GLfloat _change = yChange;
	yChange = 0.0f;
	return _change;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}