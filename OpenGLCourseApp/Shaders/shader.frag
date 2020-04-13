
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                    

out vec4 color;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
};
                                                                
uniform sampler2D theTexture;   // This uses active texture set in Texture::UseTexture() 
uniform DirectionalLight directionalLight;

void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    // color = texture(theTexture, TexCoord) * vCol;      // TO multiple color and texure            
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;                        
    color = texture(theTexture, TexCoord) * ambientColor; 
}                                                               

// Color Multiplication
/*
    vec4 a = vec4(1.0, 2.0, 3.0, 4.0);
    vec4 b = vec4(0.1, 0.2, 0.3, 0.4);
    vec4 c = a * b; // vec4(0.1, 0.4, 0.9, 1.6);
*/