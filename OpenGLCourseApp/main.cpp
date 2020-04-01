#include <iostream>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shader, uniformXMove;    // IDs for VAO, VBO, shader stored as ints

bool direction = false;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.05f;

// Vertex Shader
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
uniform float xMove;                                            \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);   \n\
}                                                               ";

// Fragment Shader
static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 color;                                                 \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);                           \n\
}                                                               ";

// This function assigns the compiled shaders to vertex
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    // error codes and result
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);    // Gets status of 'shader' program
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error Compiling the " << shaderType << " shader " << eLog << std::endl;
        return;
    }

    glAttachShader(theProgram, theShader);
}

// This function only tells opengl to compile the assigned shaders and not assign them to any vertex
void CompileShaders()
{
    shader = glCreateProgram(); // creates 'shader' as program

    if (!shader) {
        std::cout << "Error creating shader program" << std::endl;
        return;
    }

    // The below 4 line of code detects error in shaders code
    // Specifying Type of shader and its code
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    // error codes and result
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);  // Links 'shader' program with id
    glGetProgramiv(shader, GL_LINK_STATUS, &result);    // Gets status of 'shader' program
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error Linking Program: " << eLog << std::endl;
        return;
    }

    glValidateProgram(shader);  // If current program is in context with opengl version
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);    // Gets status of 'shader' program
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error Validating Program: " << eLog << std::endl;
        return;
    }

    uniformXMove = glGetUniformLocation(shader, "xMove");   // Get id of uniform variable declared in vertex shader
}

void CreateTriangle()
{
    // Center of screen is (0, 0, 0)
    // Triangle vertices in (x, y, z) order
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Starting To Create Vertex Buffer from Points for Triangle
    glGenVertexArrays(1, &VAO); // This assigns ID to this VAO 
    glBindVertexArray(VAO); 

        glGenBuffers(1, &VBO);  // This assign ID to this VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);   // Unbinding Buffer from previous buffer and Binding it to Nothing

    glBindVertexArray(0);   // Unbinding Current Vertex array to null

}

int main()
{
    // initialise GLFW
    if (!glfwInit()) {
        std::cout << "GLFW Initialisation Failed!!!" << std::endl;
        glfwTerminate();    // To terminate any operation done by glfw
        return 1;   // sends message to kernal that program crashed instead of running succesfully
    }

    //  Setup GLFW Window properties.
    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //// Core profile = No backwards compatibility
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //// Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);    // Used to create window instance
    if (!mainWindow) {
        std::cout << "GLFW window creation failed!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Set viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);    // This is OpenGL thing

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Get and handle user input events
        glfwPollEvents();

        if (direction) {
            triOffset += triIncrement;
        }
        else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }

        // Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // clears window and fresh start!!!
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);   // using 'shader' id as program to run
        glUniform1f(uniformXMove, triOffset);   // Assigning value in vertex shader by using the uniform value ID
            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        glUseProgram(0);    // Unassigning to null

        glfwSwapBuffers(mainWindow);    // Buffer swapping, You know it bhaikko, You read it in books
    }


    return 0;
}

