
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;                                                   
                                                                
out vec4 color;                                                 
                                                                
void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    color = vCol;                                               
}                                                               
