#include "pch.h"
#include "Window.h"


Window::Window() {
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
}

GLfloat Window::GetBufferWidth() {
	return (GLfloat)bufferWidth;
}

GLfloat Window::GetBufferHeight() {
	return (GLfloat)bufferHeight;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(mainWindow);
}

int Window::Initialize() {
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// setup GLFW window properties with OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// no backwards compatibility/no deprecated features
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);				// allows forward compatibility

	mainWindow = glfwCreateWindow(width, height, "Happy Window (:", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);			// sets window as main window, where everything will be drew
	glewExperimental = GL_TRUE;					// allows modern extension features

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// sets the size of drawable window (usually full)
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
