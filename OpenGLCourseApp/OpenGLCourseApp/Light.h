#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, 
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

private:
	GLfloat ambientIntensity;
	glm::vec3 colour;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

