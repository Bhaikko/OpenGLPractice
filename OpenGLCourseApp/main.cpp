#define STB_IMAGE_IMPLEMENTATION    // Decleration for stb_image.h

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265 / 180.0f;    // GLM library accepts radians so need to convert all degree angle

Window mainWindow;
std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Delta Time Setup
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

 //Vertex Shader Path
static const char* vShader = "Shaders/shader.vert";

 //Fragment Shader Path
static const char* fShader = "Shaders/shader.frag";

GLuint  uniformModel, uniformProjection, uniformView, uniformEyePosition,
        uniformSpecularIntensity, uniformShininess,
        uniformOmniLightTransform, uniformOmniLightPos, uniformFarPlane;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3) // i+=3 because we will average out based on one triangle
    {
        unsigned int in0 = indices[i] * vLength;    // This allow to jump to that Vertex Starting defined in vertices by using its index defined in indices
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += normalOffset;    in1 += normalOffset;    in2 += normalOffset;

        vertices[in0] += normal.x;  vertices[in0 + 1] += normal.y;  vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x;  vertices[in1 + 1] += normal.y;  vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x;  vertices[in2 + 1] += normal.y;  vertices[in2 + 2] += normal.z;
    }

    // normalising Normal of each vertex
    for (size_t i = 0; i < vertexCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);

        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    // Center of screen is (0, 0, 0)

    // Adding references of vertices defined below through indexes
    unsigned int indices[] = {
        0, 3, 1,        // side triangle
        1, 3, 2,        // Each number indicates index of triangle vertices in vertices array
        2, 3, 0,
        0, 1, 2
    };


    // Triangle vertices in (x, y, z, u, v, nx, ny, nx) order
    GLfloat vertices[] = {
        -1.0f,  -1.0f,  -0.6f,   0.0f,   0.0f,   0.0f, 0.0f, 0.0f,   // These normals will be averaged out later
        0.0f,   -1.0f,  1.0f,   0.5f,   0.0f,   0.0f, 0.0f, 0.0f,
        1.0f,   -1.0f,  -0.6f,   1.0f,   0.0f,   0.0f, 0.0f, 0.0f,
        0.0f,   1.0f,   0.0f,   0.5f,   1.0f,   0.0f, 0.0f, 0.0f,
    };  // These vertices will be stored in vertex index buffer and will be referenced to build geometry

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    // Defining Floor vertices and indices array
    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0, -1.0f, 0.0f,    // 10.0f as UV value means it wll be repeated for 10 times
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0, -1.0f, 0.0f
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(floor);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    // Adding Shadowmap Shader
    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
    omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

GLfloat currentX = 2.5f;
GLfloat offset = 0.0f;

void RenderScene()
{
    glm::mat4 model(1.0f);    // Creating Identity Matrix

    model = glm::translate(model, glm::vec3(currentX + offset, 0.0f, -2.5f));   // Copying [triOffset, 0, 0] vector as translation in model
    //model = glm::rotate(model, angleOffset * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

    //glUniform1f(uniformXMove, triOffset);   // Assigning value in vertex shader by using the uniform value ID
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));   // value_ptr is used to get pointer of matrix as in glm, matrix isnt stored directly as pointer
    brickTexture.UseTexture();  // Use the loaded texture
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{   
    // Directional Shadow Map pass for its framebuffer
    directionalShadowShader.UseShader();

    // To make viewport and shadowmap of same dimensions
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    // Entering wrtie mode for our shadow map frame buffer
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.GetModelLocation();
    directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void OmniShadowMapPass(PointLight* light)
{
    // Directional Shadow Map pass for its framebuffer
    omniShadowShader.UseShader();

    // To make viewport and shadowmap of same dimensions
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    // Entering wrtie mode for our shadow map frame buffer
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());

    omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    shaderList[0].UseShader();   // using 'shader' id as program to run

    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();

    // Since Directional Light IDs are handled by Shader itself, Hence the below IDs are useless
    /*
        uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
        uniformDirection = shaderList[0].GetDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
    */

    glViewport(0, 0, 1280, 720);


    // Clear Window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // clears window and fresh start!!!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Combining frame and depth buffer

    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

    // Setting Up uniform values of oights in shader
    //mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);
    shaderList[0].SetDirectionalLight(&mainLight);
    shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

    // Settng up uniform value for shadowMap for directional light
    shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

    mainLight.GetShadowMap()->Read(GL_TEXTURE2);
    shaderList[0].SetTexture(1);
    shaderList[0].SetDirectionalShadowMap(2);

    glm::vec3 lowerLight = camera.GetCameraPosition();
    //lowerLight.y -= 0.7f;
    spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());

    RenderScene();

}

int main()
{
    mainWindow = Window(1280, 720);
    mainWindow.initialize();
    
    CreateObjects();
    CreateShaders();
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 1.0f);

    std::string texturePath = "Textures/brick2.png";
    brickTexture = Texture((char* )texturePath.c_str());
    brickTexture.LoadTexture();

    texturePath = "Textures/dirt.png";
    dirtTexture = Texture((char* )texturePath.c_str());
    dirtTexture.LoadTexture();


    // Directional Light
    mainLight = DirectionalLight(
        2048, 2048,
        1.0f, 1.0f, 1.0f, 
        0.1f,   // Ambient Light CFG
        1.0f,   // Diffuse Light CFG
        0.0f, -15.0f, -10.0f
    );


    pointLights[0] = PointLight(
        1024, 1024,
        0.01f, 100.0f,
        0.0f, 0.0f, 1.0f,
        0.1f, 1.0f,
        4.0f, 2.0f, 0.0f,
        0.3f, 0.1f, 0.1f
    );
    pointLightCount++;

    pointLights[1] = PointLight(
        1024, 1024,
        0.01, 100.0f,
        0.0f, 1.0f, 0.0f,
        0.1f, 1.0f,
        -4.0f, 3.0f, 0.0f,
        0.3f, 0.1f, 0.1f
    );
    pointLightCount++;

    spotLights[0] = SpotLight(
        1024, 1024,
        0.01, 100.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,
        4.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.3f, 0.2f, 0.1f,
        20.0f
    );
    spotLightCount++;

    shinyMaterial = Material(1.0f, 32.0f);
    dullMaterial = Material(0.3f, 4.0f);

    // Accepts angle as radians not degress
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Loop until window closed
    while (!mainWindow.GetShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        offset = sin(glfwGetTime()) * 200.0f * deltaTime;


        // Get and handle user input events
        glfwPollEvents();

        camera.KeyControl(mainWindow.getKeys(), deltaTime);    // Passing reference of keys to Camera
        camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // This will write shadowmap to a frame buffer
        // Which will be used as texture and will be mapped 
        // using Fragment shader
        DirectionalShadowMapPass(&mainLight);
        
        for (size_t i = 0; i < pointLightCount; i++) {
            OmniShadowMapPass(&pointLights[i]);
        }
        
        for (size_t i = 0; i < spotLightCount; i++) {
            OmniShadowMapPass(&spotLights[i]);
        }

        RenderPass(projection, camera.CalculateViewMatrix());
        
        glUseProgram(0);    // Unassigning to null

        mainWindow.SwapBuffers();    // Buffer swapping, You know it bhaikko, You read it in books
    }


    return 0;
}                                                    