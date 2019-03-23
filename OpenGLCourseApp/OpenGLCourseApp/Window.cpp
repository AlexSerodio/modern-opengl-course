#include "pch.h"
#include "Window.h"


Window::Window() {
	xChange = 0.0f;
	yChange = 0.0f;
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	xChange = 0.0f;
	yChange = 0.0f;
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	mouseFirstMoved = true;
}

GLfloat Window::GetBufferWidth() {
	return (GLfloat)bufferWidth;
}

GLfloat Window::GetBufferHeight() {
	return (GLfloat)bufferHeight;
}

bool* Window::GetKeys() {
	return keys;
}

GLfloat Window::GetXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::GetYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
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
	
	CreateCallbacks();							// handle key and mouse input
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(mainWindow, this);		

	return 0;
}

void Window::CreateCallbacks() {
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		} else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
