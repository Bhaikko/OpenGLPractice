#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0;
	bufferHeight = 0;
	mainWindow = 0;
    mouseFirstMoved = true;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
    mouseFirstMoved = true;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
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

    // Handle Key + Mouse Input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    // Lock cursor to 'mainWindow'

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

    // When callback for handleKeys is called, the function is static, hence, it doesnt know which window to refer.
    // Hence when openGL calls the callback, the below function will set the pointer of opened window
    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

void Window::createCallbacks()
{
    // Set callback function when key event occurs
    glfwSetKeyCallback(mainWindow, handleKeys);

    // Set mouse callback
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));   // Get pointer set to mainWindow in above funtion
    
    // Alternative way to get out of window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key < 1024 && key >= 0) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    /*std::cout << theWindow->xChange << " " << theWindow->yChange << std::endl;*/
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

