#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;

public:
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
};

