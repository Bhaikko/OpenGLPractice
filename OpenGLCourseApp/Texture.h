#pragma once

#include <iostream>

#include <GL/glew.h>

#include "stb_image.h"

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