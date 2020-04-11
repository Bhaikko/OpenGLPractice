
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                                
                                                                
out vec4 color;            

uniform sampler2D theTexture;   // This uses active texture set in Texture::UseTexture() 
                                                                
void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    color = texture(theTexture, TexCoord);                                          
}                                                               
