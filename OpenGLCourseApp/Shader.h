#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
	Shader();
	void CreateFromFiles(const char* vShader, const char* fShader);
	void UseShader() { glUseProgram(this->shader); }
	
	~Shader();

private:
	GLuint shader, uniformModel, uniformProjection, uniformView, uniformEyePosition,
		uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection,	// Directional Light IDs
		uniformSpecularIntensity, uniformShininess;													// Material IDs
	std::string readShaderFromFile(const char* shaderPath);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileShaders(const char* vShaderCode, const char* fShaderCode);

public:
	GLuint GetModelLocation() { return this->uniformModel; }
	GLuint GetProjectionLocation() { return this->uniformProjection; }
	GLuint GetViewLocation() { return this->uniformView; }
	GLuint GetAmbientIntensityLocation() { return this->uniformAmbientIntensity; }
	GLuint GetAmbientColorLocation() { return this->uniformAmbientColor; }
	GLuint GetDirectionLocation() { return this->uniformDirection; }
	GLuint GetDiffuseIntensityLocation() { return this->uniformDiffuseIntensity; }
	GLuint GetSpecularIntensityLocation() { return this->uniformSpecularIntensity; }
	GLuint GetShininessLocation() { return this->uniformShininess; }
	GLuint GetEyePositionLocation() { return this->uniformEyePosition; }

};

