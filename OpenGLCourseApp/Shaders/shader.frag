
// Fragment Shader
                               
#version 330                                                    
                                                                
in vec4 vCol;   
in vec2 TexCoord;                                    
in vec3 Normal;
in vec3 FragPos;

// For shadow Mapping
in vec4 DirectionalLightSpacePos;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight {
    PointLight base;    // Since Spotlight is after all a spotlight with strict area to illuminate
    vec3 direction;
    float edge;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;
                                                                
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;           // This uses active texture set in Texture::UseTexture() 
uniform sampler2D directionalShadowMap; // Texture to apply after shadow mapping

uniform Material material;

uniform vec3 eyePosition;   // Camera Position

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    // Maps x, y, z coordinates between [-1, 1]
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;

    // Normalising projCoords from [-1, 1] => [0, 1];
    // Depth map uses values between [0, 1]
    projCoords = (projCoords * 0.5) + 0.5;

    // Working using Orthogonal Projection
    // Tells if how it move w.r.t light when moved in x and y direction
    float closetDepth = texture(directionalShadowMap, projCoords.xy).r;

    // Forward and backward values;
    float currentDepth = projCoords.z;  

    // Comparison of two objects, 
    // Wheather they are on same depth or not
    // 1.0f is full shadow, 0.0f is no shadow
    float shadow = currentDepth > closetDepth ? 1.0f : 0.0f; 

    return shadow;
}


// Since Light based on direction is mandatory to be calculated for both 
// Directional Light and Point
// Only difference is, Point light attenuates
vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
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

    return ambientColor + ((1.0f - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = CalcLightByDirection(pLight.base, direction, 0.0f);

    // attenuation = ax ^ 2 + bx + c;
    float attenuation = pLight.exponent * distance * distance + 
                        pLight.linear * distance + 
                        pLight.constant;

    return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
    // Spot light calculation based on Vector Maths
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);   

    if (slFactor > sLight.edge) {
        vec4 color = CalcPointLight(sLight.base);

        // Retuns a smooth edge around spotlight
        return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
    } else {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < pointLightCount; i++) {
        totalColor += CalcPointLight(pointLights[i]);
    }

    return totalColor;
}

vec4 CalcSpotLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spotLightCount; i++) {
        totalColor += CalcSpotLight(spotLights[i]);
    }

    return totalColor;
}

void main()                                                     
{                                                               
    // color = vec4(1.0, 0.0, 0.0, 1.0);                        
    // color = vCol;     
    // color = texture(theTexture, TexCoord) * vCol;      // To multiply color and texure            
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights(); 
    finalColor += CalcSpotLights();

    color = texture(theTexture, TexCoord) * finalColor; 
}                                                               

// Color Multiplication
/*
    vec4 a = vec4(1.0, 2.0, 3.0, 4.0);
    vec4 b = vec4(0.1, 0.2, 0.3, 0.4);
    vec4 c = a * b; // vec4(0.1, 0.4, 0.9, 1.6);
*/