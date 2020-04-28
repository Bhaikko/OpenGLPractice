#version 330

in vec3 TexCoords;

out vec4 color;

uniform samplerCube skyBox;

void main()
{
    color = Texture(skyBox, TexCoords);
}