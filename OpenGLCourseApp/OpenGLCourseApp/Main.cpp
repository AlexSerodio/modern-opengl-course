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
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "Light.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
	//	  x      y     z     u     v
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f, 0.5f, 1.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vertexShader, fragmentShader);
	shaderList.push_back(*shader1);
}

int main() {
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	GLuint uniformAmbientIntensity = 0, uniformAmbientColour = 0;

	GLfloat aspectRatio = mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight();
	glm::mat4 projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);

	while (!mainWindow.ShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();	// gets and handle user input events

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// clear window and depth buffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// sets the shader that will be used to draw the object
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);

		glm::mat4 model = glm::mat4(1.0f);		// defines a 4x4 identity matrix

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);		// unset shader

		mainWindow.SwapBuffers();
	}

	return 0;
} 