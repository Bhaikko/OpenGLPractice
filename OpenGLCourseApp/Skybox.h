#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"

#include "CommonValues.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	~Skybox();

private:
	// Since skybox has its own shader and mesh geometry
	Mesh* skyMesh;
	Shader* skyShader;

private:
	GLuint textureId;
	GLuint uniformProjection, uniformView;

public:
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
};

