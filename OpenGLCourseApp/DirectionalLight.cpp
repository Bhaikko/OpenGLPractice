#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	// Since Super Contructor, Light() is called, 
	// we dont need to set color and intensities here, 
	// but will be set in Light()

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity,	GLfloat dIntensity,	
	GLfloat xDir, GLfloat yDir, GLfloat zDir
) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
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
