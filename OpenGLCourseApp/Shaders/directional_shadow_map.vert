#version 330

layout (location = 0) in vec3 pos; 

// We only need to know, where things are
// In relation to lighting

uniform mat4 model;

// Used to transform vertex pos in space of Light source
// Combination of Projection and View matrix
uniform mat4 directionalLightTransform;   

void main()
{
    gl_Position = directionalLightTransform * model * vec4(pos, 1.0f);
}