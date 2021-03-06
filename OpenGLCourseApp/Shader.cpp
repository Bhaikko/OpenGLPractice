#include "Shader.h"

Shader::Shader()
{
	shader = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromFiles(const char* vShader, const char* fShader)
{
	std::string vShaderCode = readShaderFromFile(vShader);
	std::string fShaderCode = readShaderFromFile(fShader);

	CompileShaders(vShaderCode.c_str(), fShaderCode.c_str());
}

void Shader::CreateFromFiles(const char* vShader, const char* gShader, const char* fShader)
{
	std::string vShaderCode = readShaderFromFile(vShader);
	std::string gShaderCode = readShaderFromFile(gShader);
	std::string fShaderCode = readShaderFromFile(fShader);

	CompileShaders(vShaderCode.c_str(), gShaderCode.c_str(), fShaderCode.c_str());
}



Shader::~Shader()
{

}

std::string Shader::readShaderFromFile(const char* shaderPath)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		// Convert Streams to GLchar Array
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Shader File " << shaderPath << " cannot be read " << std::endl;
	}

	return code;
}

// This function assigns the compiled shaders to vertex
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// error codes and result
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);    // Gets status of 'shader' program
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling the " << shaderType << " shader " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

// This function only tells opengl to compile the assigned shaders and not assign them to any vertex
void Shader::CompileShaders(const char* vShaderCode, const char* fShaderCode)
{
	shader = glCreateProgram(); // creates 'shader' as program

	if (!shader) {
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	// The below 4 line of code detects error in shaders code
	// Specifying Type of shader and its code
	AddShader(shader, vShaderCode, GL_VERTEX_SHADER);
	AddShader(shader, fShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();

}

void Shader::CompileShaders(const char* vShaderCode, const char* gShaderCode, const char* fShaderCode)
{
	shader = glCreateProgram(); // creates 'shader' as program

	if (!shader) {
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	// The below 4 line of code detects error in shaders code
	// Specifying Type of shader and its code
	AddShader(shader, vShaderCode, GL_VERTEX_SHADER);
	AddShader(shader, gShaderCode, GL_GEOMETRY_SHADER);
	AddShader(shader, fShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();

}

void Shader::Validate()
{
	// error codes and result
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shader);  // If current program is in context with opengl version
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);    // Gets status of 'shader' program
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: " << eLog << std::endl;
		return;
	}
}

void Shader::CompileProgram()
{
	// error codes and result
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);  // Links 'shader' program with id
	glGetProgramiv(shader, GL_LINK_STATUS, &result);    // Gets status of 'shader' program
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program: " << eLog << std::endl;
		return;
	}

	

	// Uniform values of Transform Matrices
	uniformModel = glGetUniformLocation(shader, "model");   // Get id of uniform variable declared in vertex shader using current vertex shader id
	uniformProjection = glGetUniformLocation(shader, "projection");
	uniformView = glGetUniformLocation(shader, "view");
	uniformEyePosition = glGetUniformLocation(shader, "eyePosition");

	// Uniform values of Directional Light
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shader, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shader, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shader, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shader, "directionalLight.direction");


	// Uniform values of Point Lights
	uniformPointLightCount = glGetUniformLocation(shader, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		std::string uniformLocationString = "";

		uniformLocationString = "pointLights[" + std::to_string(i) + "].base.color";
		uniformPointLight[i].uniformColor = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].base.ambientIntensity";
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].base.diffuseIntensity";
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].position";
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].constant";
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].linear";
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "pointLights[" + std::to_string(i) + "].exponent";
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shader, uniformLocationString.c_str());
	}

	// Uniform Values of Spot Lights
	uniformSpotLightCount = glGetUniformLocation(shader, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		std::string uniformLocationString = "";

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.base.color";
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.base.ambientIntensity";
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.base.diffuseIntensity";
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.position";
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.constant";
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.linear";
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].base.exponent";
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].direction";
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "spotLights[" + std::to_string(i) + "].edge";
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shader, uniformLocationString.c_str());
	}

	// Uniform values of material
	uniformSpecularIntensity = glGetUniformLocation(shader, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shader, "material.shininess");

	// Uniform values of Textures
	uniformTexture = glGetUniformLocation(shader, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shader, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shader, "directionalShadowMap");

	// Uniform values for Cube Map
	uniformOmniLightPos = glGetUniformLocation(shader, "lightPos");
	uniformFarPlane = glGetUniformLocation(shader, "farPlane");

	for (size_t i = 0; i < 6; i++) {
		std::string uniformLocationString = "";

		uniformLocationString = "lightMatrices[" + std::to_string(i) + "]";
		uniformLightMatrices[i] = glGetUniformLocation(shader, uniformLocationString.c_str());
	}

	for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++) {
		std::string uniformLocationString = "";

		uniformLocationString = "omniShadowMaps[" + std::to_string(i) + "].shadowMap";
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shader, uniformLocationString.c_str());

		uniformLocationString = "omniShadowMaps[" + std::to_string(i) + "].farPlane";
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shader, uniformLocationString.c_str());
	}
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	// ID location of Directional Light are passed here and handled by shader and no longer in main()
	dLight->UseLight(
		uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, 
		uniformDirectionalLight.uniformDirection
	);
}

void Shader::SetPointLights(PointLight* pLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) {
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		pLights[i].UseLight(
			uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity,
			uniformPointLight[i].uniformPosition, 
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent
		);

		pLights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLights[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(SpotLight* sLights, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) {
		lightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		sLights[i].UseLight(
			uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge
		);

		sLights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLights[i].GetFarPlane());
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}

}

