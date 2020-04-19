#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
	Shader();
	void CreateFromFiles(const char* vShader, const char* fShader);
	void UseShader() { glUseProgram(this->shader); }
	
	~Shader();

private:
	GLuint shader, uniformModel, uniformProjection, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;													// Material IDs
	std::string readShaderFromFile(const char* shaderPath);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileShaders(const char* vShaderCode, const char* fShaderCode);

public:
	GLuint GetModelLocation() { return this->uniformModel; }
	GLuint GetProjectionLocation() { return this->uniformProjection; }
	GLuint GetViewLocation() { return this->uniformView; }
	GLuint GetAmbientIntensityLocation() { return this->uniformDirectionalLight.uniformAmbientIntensity; }
	GLuint GetAmbientColorLocation() { return this->uniformDirectionalLight.uniformColor; }
	GLuint GetDirectionLocation() { return this->uniformDirectionalLight.uniformDirection; }
	GLuint GetDiffuseIntensityLocation() { return this->uniformDirectionalLight.uniformDiffuseIntensity; }
	GLuint GetSpecularIntensityLocation() { return this->uniformSpecularIntensity; }
	GLuint GetShininessLocation() { return this->uniformShininess; }
	GLuint GetEyePositionLocation() { return this->uniformEyePosition; }


// Light Configurations
private:
	int pointLightCount;

	// Storing light IDs in struct
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

public:
	void SetDirectionalLight(DirectionalLight* dLight);
};

