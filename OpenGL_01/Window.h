#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Init();
	GLint getBufferWidth() { return bufferWidth; };
	GLint getBufferHeight() { return bufferHeight; };
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); };
	void swapBuffers() { glfwSwapBuffers(mainWindow); };

	bool* getKeys() { return keys; };
	bool* getMouseButtons() { return mouseButtons; };
	GLfloat GetXChange();
	GLfloat GetYChange();

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];
	bool mouseButtons[3];
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

