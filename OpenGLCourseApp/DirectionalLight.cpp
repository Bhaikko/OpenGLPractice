#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	// Since Super Contructor, Light() is called, 
	// we dont need to set color and intensities here, 
	// but will be set in Light()

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.f);
}

DirectionalLight::DirectionalLight(
	GLfloat shadowWidth, GLfloat shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity,	GLfloat dIntensity,	
	GLfloat xDir, GLfloat yDir, GLfloat zDir
) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::~DirectionalLight()
{
	
}

void DirectionalLight::UseLight(
	GLfloat ambientLightIntensity, GLfloat ambientColorLocation,
	GLfloat diffuseIntensityLocation, GLfloat diffuseDirectionLocation
)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);	// Binding color to ID for shader
	glUniform1f(ambientLightIntensity, ambientIntensity);

	glUniform3f(diffuseDirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	// Return Transform matrix where Projection and view matrix are combined already
	// and not calculated in vertex shader

	return	lightProj *		// Orthogonal Matrix
			glm::lookAt(	// View Matrix
				-direction, 
				glm::vec3(0.0f, 0.0f, 0.0f), 
				glm::vec3(0.0f, 1.0f, 0.0f)
			);	
}
