#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(
			GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,	// Ambient Lighting Params
			GLfloat dIntensity	// Diffuse Lighting Params
		);
	~Light();

protected:

	// Ambient Lighting 
	glm::vec3 color;
	GLfloat ambientIntensity;

	// Diffuse Lighting
	GLfloat diffuseIntensity;

};

