#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	GLfloat GetBufferWidth();
	GLfloat GetBufferHeight();
	bool ShouldClose();
	void SwapBuffers();
	int Initialize();

	~Window();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

