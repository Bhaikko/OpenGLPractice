#pragma once

#include <iostream>
#include "CommonValues.h"

#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;	// ID assigned by OpenGL
	int width, height, bitDepth;

	char* fileLocation;
};

// Texturer.com for downloading free textures