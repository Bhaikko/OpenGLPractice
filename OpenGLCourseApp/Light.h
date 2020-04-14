#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(
			GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,	// Ambient Lighting Params
			GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity	// Diffuse Lighting Params
		);
	~Light();

private:

	// Ambient Lighting 
	glm::vec3 color;
	GLfloat ambientIntensity;

	// Diffuse Lighting
	glm::vec3 direction;
	GLfloat diffuseIntensity;

public:
	void UseLight(
		GLfloat ambientLightIntensity, GLfloat ambientColorLocation,	// Ambient Light IDs
		GLfloat diffuseIntensityLocation, GLfloat diffuseDirectionLocation
	);
};

