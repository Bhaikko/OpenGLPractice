// Vertex Shader
                                
#version 330                                                    
                                                                
layout (location = 0) in vec3 pos;          // In glVertexAttribPointer, first argument, index referes to this location                    
layout (location = 1) in vec2 tex;

out vec4 vCol;                     
out vec2 TexCoord;                             
                                                                
uniform mat4 model;                                             
uniform mat4 projection;   
uniform mat4 view;   
                                                                
void main()                                                     
{                                                               
    gl_Position = projection * view * model * vec4(pos, 1.0);                       
    // gl_Position = vec4(pos, 1.0);           
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);    

    TexCoord = tex;              
}                                                               
