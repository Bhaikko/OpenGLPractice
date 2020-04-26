// Geometry shader
// Handles multiple vertices which form Primitives

#version 330

layout (triangles) in;                          // Input of geometry shader
layout (traingle_strip, max_vertices = 18) out; // max_vertices to out of this shader = 18, output

uniform mat4 lightMatrices[6];

out vec4 FragPos;

void main()
{
    // Iterating over each face
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;

        // Iterating over each vertex of triangle passed into this geometry
        for (int i = 0; i < 3; i++) {
            FragPos = gl_in[i].gl_Position;

            // Changing basis of each vertex by using each face Transformation matrix
            gl_Position = lightMatrices[face] * FragPos;    
            EmitVertex();
        }

        EndPrimitive();
    }
}