#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity
)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

Light::~Light()
{

}

void Light::UseLight(
	GLfloat ambientLightIntensity, GLfloat ambientColorLocation,
	GLfloat diffuseIntensityLocation, GLfloat diffuseDirectionLocation
)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);	// Binding color to ID for shader
	glUniform1f(ambientLightIntensity, ambientIntensity);

	glUniform3f(diffuseDirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

