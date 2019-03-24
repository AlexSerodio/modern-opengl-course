#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void Cleartexture();

	~Texture();

private:
	GLuint textureID;
	int width, heigth, bitDepth;

	const char* fileLocation;
};

