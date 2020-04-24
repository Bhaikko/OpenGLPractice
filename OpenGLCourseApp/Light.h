#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(
			GLfloat shadowWidth, GLfloat shadowHeight,
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

	glm::mat4 lightProj;
	ShadowMap* shadowMap;

public:
	ShadowMap* GetShadowMap() { return shadowMap; }

};

