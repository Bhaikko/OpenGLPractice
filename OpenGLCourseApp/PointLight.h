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

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;	// For attenuation Equation


public:
	void UseLight(
		GLuint ambientLightIntensity, GLuint ambientColorLocation,	// Ambient Light IDs
		GLuint diffuseIntensityLocation,								// Diffuse Light ID
		GLuint positionLocation,										// Point Light Location ID
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation
	);
};

