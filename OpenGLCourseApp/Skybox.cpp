#include "Skybox.h"

Skybox::Skybox()
{

}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// Shader Setup
	skyShader = new Shader();
	skyShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();

	// Texture Setup
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, bitDepth;

	for (size_t i = 0; i < 6; i++) {
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData) {
			std::cout << "Failed to find " << faceLocations[i].c_str() << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh Setup Code for the vertices of a cube
	unsigned int skyboxIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	// Since in cube maps, Texels are refered by vector not YV coordinates
	// Hence (0, 0) for UV coordinates
	// as well as no lighting effects, Hence no normals
	GLfloat skyboxVertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	skyMesh = new Mesh();
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

Skybox::~Skybox()
{

}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));	// removing the translation from view Matrix

	// Disabling Depth mask
	glDepthMask(GL_FALSE);

	skyShader->UseShader();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	skyShader->Validate();

	skyMesh->RenderMesh();

	// enabling depth mask so it can be used when rendering other things in the scene
	glDepthMask(GL_TRUE);
}
