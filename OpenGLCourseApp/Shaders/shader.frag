
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                    
in vec3 Normal;
in vec3 FragPos;

const int MAX_POINT_LIGHTS = 3;

out vec4 color;

struct Light {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
                                                                
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;   // This uses active texture set in Texture::UseTexture() 
uniform Material material;

uniform vec3 eyePosition;   // Camera Position

// Since Light based on direction is mandatory to be calculated for both 
// Directional Light and Point
// Only difference is, Point light attenuates
vec4 CalcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuseFactor > 0.0f) {
        // Only to add specular light if there is diffuse light on Mesh

        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    } 

    return ambientColor + diffuseColor + specularColor;
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{

}

void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    // color = texture(theTexture, TexCoord) * vCol;      // TO multiple color and texure            
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights;

    color = texture(theTexture, TexCoord) * finalColor; 
}                                                               

// Color Multiplication
/*
    vec4 a = vec4(1.0, 2.0, 3.0, 4.0);
    vec4 b = vec4(0.1, 0.2, 0.3, 0.4);
    vec4 c = a * b; // vec4(0.1, 0.4, 0.9, 1.6);
*/