#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
}

ShadowMap::~ShadowMap()
{
	if (FBO) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap) {
		glDeleteTextures(1, &shadowMap);
	}
}

bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &shadowMap);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);	// Loading Texture data
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float bColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			// attaching a texture image to a framebuffer object
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

			glDrawBuffer(GL_NONE);	// Setting write setting of framebuffer to NULL
			glReadBuffer(GL_NONE);	// Setting read settings of framebuffer to NULL

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			if (status != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "Framebuffer Error: " << status << std::endl;
				return false;
			}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
}

void ShadowMap::Write()
{
	// to set current frame buffer to this from the one used to draw texture on screen
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}
