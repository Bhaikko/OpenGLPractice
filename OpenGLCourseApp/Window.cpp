#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0;
	bufferHeight = 0;
	mainWindow = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
}

int Window::initialize()
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

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);    // Used to create window instance
    if (!mainWindow) {
        std::cout << "GLFW window creation failed!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
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

    // Setting Up Depth-Buffer / Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Set viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);    // This is OpenGL thing

    return 0;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}