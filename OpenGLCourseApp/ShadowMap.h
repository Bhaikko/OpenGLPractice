#pragma once

#include <GL/glew.h>
#include <iostream>

// Gonna Hold info about texture map and frame buffer
// This is responsible for Directional Light and 
// For Point and Spot lights will be inherited by this.
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

protected:
	GLuint FBO;			// ID for Frame Buffer
	GLuint shadowMap;	// ID for shadow map as texture
	unsigned int shadowWidth, shadowHeight;

public:

	// Used to setup Framebuffer
	virtual bool Init(unsigned int width, unsigned int height);

	virtual void Write();

	//	Texture unit specifies which texture unit to bind the texture, since we will be using multiple textures now
	virtual void Read(GLenum textureUnit);		

public:
	unsigned int GetShadowWidth() { return shadowWidth; }
	unsigned int GetShadowHeight() { return shadowHeight; }
};

