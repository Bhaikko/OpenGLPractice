#pragma once

#include "Light.h"

class DirectionalLight : Light
{
public:
	DirectionalLight();

	DirectionalLight(
		GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat aIntensity,	// Ambient Lighting Params
		GLfloat dIntensity,	// Diffuse Lighting Params
		GLfloat xDir, GLfloat yDir, GLfloat zDir
	);

	~DirectionalLight();

private:
	glm::vec3 direction;

public:
	void UseLight(
		GLfloat ambientLightIntensity, GLfloat ambientColorLocation,	// Ambient Light IDs
		GLfloat diffuseIntensityLocation, GLfloat diffuseDirectionLocation
	);
};

