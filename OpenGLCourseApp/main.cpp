#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265 / 180.0f;    // GLM library accepts radians so need to convert all degree angle

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

// Delta Time Setup
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

 //Vertex Shader Path
static const char* vShader = "Shaders/shader.vert";

 //Fragment Shader Path
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
    // Center of screen is (0, 0, 0)

    // Adding references of vertices defined below through indexes
    unsigned int indices[] = {
        0, 3, 1,    // side triangle
        1, 3, 2,     // Each number indicates index of triangle vertices in vertices array
        2, 3, 0,
        0, 1, 2
    };


    // Triangle vertices in (x, y, z) order
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };  // These vertices will be stored in vertex index buffer and will be referenced to build geometry

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow.initialize();
    
    CreateObjects();
    CreateShaders();
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 1.0f);

    GLuint uniformModel, uniformProjection, uniformView;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

    bool a = false;

    // Loop until window closed
    while (!mainWindow.GetShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // Get and handle user input events
        glfwPollEvents();

        camera.KeyControl(mainWindow.getKeys(), deltaTime);    // Passing reference of keys to Camera
        camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // clears window and fresh start!!!
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Combining frame and depth buffer

        shaderList[0].UseShader();   // using 'shader' id as program to run
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

        glm::mat4 model(1.0f);    // Creating Identity Matrix
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));   // Copying [triOffset, 0, 0] vector as translation in model
        //model = glm::rotate(model, angleOffset * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        
        //glUniform1f(uniformXMove, triOffset);   // Assigning value in vertex shader by using the uniform value ID
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));   // value_ptr is used to get pointer of matrix as in glm, matrix isnt stored directly as pointer
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));   // value_ptr is used to get pointer of matrix as in glm, matrix isnt stored directly as pointer
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));   // value_ptr is used to get pointer of matrix as in glm, matrix isnt stored directly as pointer
        meshList[1]->RenderMesh();

        glUseProgram(0);    // Unassigning to null

        mainWindow.SwapBuffers();    // Buffer swapping, You know it bhaikko, You read it in books
        glfwWaitEvents();   // Fixes the loading cursor bug
    }


    return 0;
}

