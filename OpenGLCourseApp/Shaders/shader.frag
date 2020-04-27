
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

struct OmniShadowMap {
    samplerCube shadowMap;
    float farPlane;
};

uniform int pointLightCount;
uniform int spotLightCount;
                                                                
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;           // This uses active texture set in Texture::UseTexture(), GL_TEXTURE0
uniform sampler2D directionalShadowMap; // Texture to apply after shadow mapping, GL_TEXTURE1
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];   // for each spot and point light present in scene

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
    // float closetDepth = texture(directionalShadowMap, projCoords.xy).x;

    // Forward and backward values;
    float currentDepth = projCoords.z;  

    // Bias is used to fix conflict between light and camera,
    // which creates an artifact/acne of block lines on geometry
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);

    float bias = max(0.05f * (1 - dot(normal, lightDir)), 0.005);

    // Supersampling to average out pixel colors
    // And to remove jaginess in shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);    // Get texture size in texels

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).x;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f; 
        }
    }

    shadow /= 9;

    // Comparison of two objects, 
    // Wheather they are on same depth or not
    // 1.0f is full shadow, 0.0f is no shadow
    // float shadow = currentDepth - bias > closetDepth ? 1.0f : 0.0f; 

    // Discard shadows that are beyond the far plane of orthographic view frustum
    if (projCoords.z > 1.0f) {
        shadow = 0.0f;
    }

    return shadow;
}

// shadowIndex is iterator to current shadow map for its respective light in omniShadowMaps
float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0f;
    float bias = 0.05f;
    float samples = 2.0f;
    float offset = 0.1f;

    // Sampling for Cube Map in 3 Dimensional
    for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closetDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + vec3(x, y, z)).x;   // Since shadowmaps have only One Value
                closetDepth *= omniShadowMaps[shadowIndex].farPlane;    // since in omni_shadow_map.frag, we normalized the closet

                if (currentDepth - bias > closetDepth) {
                    shadow += 1.0f;
                }
            }
        }
    }

    shadow /= (samples * samples * samples);
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

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);

    vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);

    // attenuation = ax ^ 2 + bx + c;
    float attenuation = pLight.exponent * distance * distance + 
                        pLight.linear * distance + 
                        pLight.constant;

    return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
    // Spot light calculation based on Vector Maths
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);   

    if (slFactor > sLight.edge) {
        vec4 color = CalcPointLight(sLight.base, shadowIndex);

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
        totalColor += CalcPointLight(pointLights[i], i);
    }

    return totalColor;
}

vec4 CalcSpotLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spotLightCount; i++) {
        totalColor += CalcSpotLight(spotLights[i], i + pointLightCount);
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