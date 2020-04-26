#pragma once

#include "Light.h"
#include "OmniShadowMap.h"

class PointLight :	public Light
{
public:
	PointLight();
	PointLight(
		GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp
	);
	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;	// For attenuation Equation

	GLfloat farPlane;

public:
	void UseLight(
		GLuint ambientLightIntensity, GLuint ambientColorLocation,	// Ambient Light IDs
		GLuint diffuseIntensityLocation,								// Diffuse Light ID
		GLuint positionLocation,										// Point Light Location ID
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation
	);

	// Since we are using Cubemap,
	// We need 6 Transformation Matrices
	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat GetFarPlane() { return farPlane; }
	glm::vec3 GetPosition() { return position; }
};

