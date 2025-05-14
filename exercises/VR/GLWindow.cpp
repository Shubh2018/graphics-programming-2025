#include "GLWindow.h"

GLWindow::GLWindow()
{
	width = 800;
	height = 600;

	xChange = 0.0f;
	yChange = 0.0f;

	for (bool b : keys)
	{
		b = false;
	}
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;

	for (bool b : keys)
	{
		b = false;
	}
}

int GLWindow::Initialize()
{
	//GLFW Initialization
	if (!glfwInit())
	{
		std::cout << "GLFW Not initialized";
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties

	//OpenGl Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Set the OpenGL major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);  // Set the OpenGL minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile = OpenGL will not be backward compatible
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OpenGL will be forward compatible

	mainWindow = glfwCreateWindow(width, height, "VoxelRenderer", NULL, NULL);

	if (!mainWindow)
	{
		std::cout << "GLFW Window Creation Failed!!";
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle key + mouse input
	CreateCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat GLWindow::GetXChange()
{
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat GLWindow::GetYChange()
{
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void GLWindow::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void GLWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	GLWindow* thisWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			thisWindow->keys[key] = true;
		}

		if(action == GLFW_RELEASE)
		{
			thisWindow->keys[key] = false;
		}
	}
}

void GLWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	GLWindow* thisWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (thisWindow->mouseFirstMove)
	{
		thisWindow->lastX = xPos;
		thisWindow->lastY = yPos;
		thisWindow->mouseFirstMove = false;
	}

	thisWindow->xChange = xPos - thisWindow->lastX;
	thisWindow->yChange = thisWindow->lastY - yPos;

	thisWindow->lastX = xPos;
	thisWindow->lastY = yPos;
}