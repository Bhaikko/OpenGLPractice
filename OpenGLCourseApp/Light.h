#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

public:
	void UseLight(GLfloat ambientLightIntensity, GLfloat ambientColorLocation);
};

