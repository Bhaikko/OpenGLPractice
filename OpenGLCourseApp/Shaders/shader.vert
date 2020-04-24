// Vertex Shader
                                
#version 330                                                    
                                                                
layout (location = 0) in vec3 pos;          // In glVertexAttribPointer, first argument, index referes to this location                    
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;         // Normal of current vertex after being averaged out by surfaces

out vec4 vCol;                     
out vec2 TexCoord;                 
out vec3 Normal; 
out vec3 FragPos;           
                                                                
uniform mat4 model;                                             
uniform mat4 projection;   
uniform mat4 view;   

// For directional shadow mapping
out vec4 DirectionalLightSpacePos;

uniform mat4 directionalLightTransform;     // To find fragment w.r.t to directional Light

                                                                
void main()                                                     
{                                                               
    gl_Position = projection * view * model * vec4(pos, 1.0);      
    // gl_Position = vec4(pos, 1.0);           
    DirectionalLightSpacePos = directionalLightTransform * model * vec(pos, 1.0f);
                     
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);    

    TexCoord = tex;

    // To transform normals of vertex when the same vertex is rotated or scaled.
    Normal = mat3(transpose(inverse(model))) * norm;   

    // swizzling to get vec3 from vec4
    FragPos = (model * vec4(pos, 1.0)).xyz;
}                                                               
