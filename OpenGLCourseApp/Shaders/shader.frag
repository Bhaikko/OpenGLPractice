
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                    
in vec3 Normal;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};
                                                                
uniform sampler2D theTexture;   // This uses active texture set in Texture::UseTexture() 
uniform DirectionalLight directionalLight;

void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    // color = texture(theTexture, TexCoord) * vCol;      // TO multiple color and texure            
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.direction, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor); 
}                                                               

// Color Multiplication
/*
    vec4 a = vec4(1.0, 2.0, 3.0, 4.0);
    vec4 b = vec4(0.1, 0.2, 0.3, 0.4);
    vec4 c = a * b; // vec4(0.1, 0.4, 0.9, 1.6);
*/