#pragma once
#include "Light.h"
class PointLight :	public Light
{
public:
	PointLight();
	PointLight(
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp
	);
	~PointLight();

private:
	glm::vec3 position;

	GLfloat constant, linear, exponent;	// For attenuation Equation


public:
	void UseLight(
		GLfloat ambientLightIntensity, GLfloat ambientColorLocation,	// Ambient Light IDs
		GLfloat diffuseIntensityLocation,								// Diffuse Light ID
		GLfloat positionLocation,										// Point Light Location ID
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation
	);
};

