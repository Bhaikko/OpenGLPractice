#include "Shader.h"

Shader::Shader()
{
	shader = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromFiles(const char* vShader, const char* fShader)
{
	std::string vShaderCode = readShaderFromFile(vShader);
	std::string fShaderCode = readShaderFromFile(fShader);

	CompileShaders(vShaderCode.c_str(), fShaderCode.c_str());
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

	glValidateProgram(shader);  // If current program is in context with opengl version
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);    // Gets status of 'shader' program
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");   // Get id of uniform variable declared in vertex shader using current vertex shader id
	uniformProjection = glGetUniformLocation(shader, "projection");   
	uniformView = glGetUniformLocation(shader, "view");   
	uniformAmbientColor = glGetUniformLocation(shader, "directionalLight.color");
	uniformAmbientIntensity = glGetUniformLocation(shader, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(shader, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shader, "directionalLight.diffuseIntensity");
}