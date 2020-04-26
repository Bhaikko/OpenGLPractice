#pragma once
#include "ShadowMap.h"
class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap();

public:
	bool Init(GLuint width, GLuint height) override;
	void Write() override;
	void Read(GLenum textureUnit) override;
};

