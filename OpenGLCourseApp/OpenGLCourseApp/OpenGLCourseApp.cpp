#include "pch.h"
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// store the IDs for each object
GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout(location = 0) in vec3 pos;								\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = vec4(0.4 * pos.x, 0.4 *	pos.y, pos.z, 1.0);	\n\
}";

// Fragment Shader
static const char* fShader = "									\n\
#version 330													\n\
																\n\
out vec4 colour;												\n\
																\n\
void main()														\n\
{																\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);							\n\
}";

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// creates a VAO, return it's id and set it as the default VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		// creates a VBO, return it's id and bind it to the VAO setted before
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

// TODO
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);		// creates an empty shader

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// handles result error
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	// handles result error
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

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

	CreateTriangle();
	CompileShaders();

	// loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {

		// get and handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// sets the shader that will be used to draw the object
		glUseProgram(shader);

			glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);

		// unset the shader
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}