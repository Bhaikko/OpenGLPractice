#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	//fileLocation = "";
}

Texture::Texture(char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		std::cout << "Failed to find " << fileLocation << std::endl;
		return;
	}

	glGenTextures(1, &textureID);	// Generating ID for current Texture
	glBindTexture(GL_TEXTURE_2D, textureID);

		// Defining Parameters for current texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Loading Texture Data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);	// loads texture which has Aplha Channel in texture
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);	// loads texture which does not Aplha Channel in texture
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);	// Unbinding currently loaded texture

	stbi_image_free(texData);	// Free data loaded in texData
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	//fileLocation = "";
}

void Texture::UseTexture()
{
	// Below code binds the texture in memory associated with TextureID to Texture0 (unit)
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture()
{
	ClearTexture();
}