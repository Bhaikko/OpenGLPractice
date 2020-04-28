#version 330

layout (location = 0) in vec3 pos;  

out vec3 TexCoords;                     // The textcoords will be same as vertex position on cube map

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = pos;
    gl_Position = projection * view * vec4(pos, 1.0f);
}