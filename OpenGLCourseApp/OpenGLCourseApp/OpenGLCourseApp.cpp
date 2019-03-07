#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

bool direction = true;		// true for right and false for left
float triOffset = 0.0f;
const float triMaxOffset = 0.7f;
const float triIncrement = 0.001f;

float curAngle = 0.0f;
const float angleIncrement = 0.02f;

bool sizeDirection = true;
float curSize = 0.4f;
const float maxSize = 0.8f;
const float minSize = 0.1f;
const float sizeIncrement = 0.0005f;

static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vertexShader, fragmentShader);
	shaderList.push_back(shader1);
}

int main() {
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0; 

	GLfloat aspectRatio = mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight();
	glm::mat4 projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);

	// loop until window closed
	while (!mainWindow.ShouldClose()) {

		// get and handle user input events
		glfwPollEvents();

		// update the offset variable depending on current position
		if (direction) {
			triOffset += triIncrement;
		} else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += (curAngle < 360) ? angleIncrement : -360;

		if (sizeDirection) {
			curSize += sizeIncrement;
		} else {
			curSize -= sizeIncrement;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		// clear window and depth buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// sets the shader that will be used to draw the object
		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model;

		model = glm::mat4(1.0);						// defines a 4x4 identity matrix
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		// model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		// unset the shader
		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}