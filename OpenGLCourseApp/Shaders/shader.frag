
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                    
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material {
    float specularIntensity;
    float shininess;
};
                                                                
uniform sampler2D theTexture;   // This uses active texture set in Texture::UseTexture() 
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;   // Camera Position

void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    // color = texture(theTexture, TexCoord) * vCol;      // TO multiple color and texure            
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuseFactor > 0.0f) {
        // Only to add specular light if there is diffuse light on Mesh

        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }    

    color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor + specularColor); 
}                                                               

// Color Multiplication
/*
    vec4 a = vec4(1.0, 2.0, 3.0, 4.0);
    vec4 b = vec4(0.1, 0.2, 0.3, 0.4);
    vec4 c = a * b; // vec4(0.1, 0.4, 0.9, 1.6);
*/