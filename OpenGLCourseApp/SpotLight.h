#pragma once
#include "PointLight.h"

// Since, Spot light is just point light which falls on a certain area,
// We use PointLight as Base class
class SpotLight : public PointLight
{

public:
	SpotLight();

	SpotLight(
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edge	// Edge is spread in degrees from Source
	);

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;

public:
	void UseLight(
		GLuint ambientLightIntensity, GLuint ambientColorLocation,	
		GLuint diffuseIntensityLocation,								
		GLuint positionLocation, GLuint directionLocation,						
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation
	);
};

