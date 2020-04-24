#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();
	void CreateFromFiles(const char* vShader, const char* fShader);
	void UseShader() { glUseProgram(this->shader); }
	
	~Shader();

private:
	GLuint shader, uniformModel, uniformProjection, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,	// Material IDs
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap;

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
	// DIRECTIONAL LIGHT
	// Storing directional light IDs in struct
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;


	// POINT LIGHT
	int pointLightCount;

	// To bind the above Point light count to shader point light count
	GLuint uniformPointLightCount;	

	// Storing Point light IDs in struct for multiple point lights
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];


	// SPOT LIGHT
	int spotLightCount;
	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

public:
	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLights, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLights, unsigned int lightCount);

	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
};

