#include "pch.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
	// initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// setup GLFW window properties
	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// no backwards compatibility / don't allow deprecated features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// get buffer size information 
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set content for GLEW to use
	// set the window as the main window where everything will be drew
	glfwMakeContextCurrent(mainWindow);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	// initialize glew
	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// setup viewport size / sets the size of drawable window (usually full)
	glViewport(0, 0, bufferWidth, bufferHeight);

	// loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {

		// get and handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}