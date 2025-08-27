#include "LunaWindow.h"

LunaWindow* LunaWindow::m_FocusedWindow = nullptr;

LunaWindow::LunaWindow()
{
	m_WindowTitle = "";
	m_WindowWidth = m_WindowHeight = 0;
}

LunaWindow::~LunaWindow()
{

}

int LunaWindow::GetKey(int key)
{
	return glfwGetKey(m_WindowHandle, key);
}

int LunaWindow::GetMouseButton(int button)
{
	return glfwGetMouseButton(m_WindowHandle, button);
}

void LunaWindow::GetCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(m_WindowHandle, x, y);
}

void LunaWindow::SetCursorPosition(double x, double y)
{
	glfwSetCursorPos(m_WindowHandle, x, y);
}

void LunaWindow::SetInputMode(int mode, int value)
{
	glfwSetInputMode(m_WindowHandle, mode, value);
}

LunaWindow* LunaWindow::NewWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight)
{
	// Test GLFW init
	if (!glfwInit())
	{
		std::cerr << "GLFW FAILED INIT" << std::endl;
		return nullptr;
	}

	// Initialize window class instance with member variables
	LunaWindow* window = new LunaWindow();

	window->m_WindowTitle = windowTitle;
	window->m_WindowWidth = windowWidth;
	window->m_WindowHeight = windowHeight;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Create and test the GLFW window handle
	window->m_WindowHandle = glfwCreateWindow(
		windowWidth, windowHeight, windowTitle,
		NULL, NULL); 

	if (!window->m_WindowHandle)
	{
		glfwTerminate();
		std::cerr << windowTitle << " - WINDOW FAILED INIT" << std::endl;
		return nullptr;
	}

	// Set GLFW window settings
	glfwMakeContextCurrent(window->m_WindowHandle);
	glfwMaximizeWindow(window->m_WindowHandle);
	glfwSwapInterval(1);

	// Init OpenGL
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}

	// Set OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Only set is running to true if all phases of init pass
	window->m_IsRunning = true;
	LunaWindow::m_FocusedWindow = window;

	return window;
}

void LunaWindow::FocusWindow()
{
	LunaWindow::m_FocusedWindow = this;
}

void LunaWindow::CloseWindow(LunaWindow* window)
{
	glfwWindowShouldClose(window->m_WindowHandle);
	glfwTerminate();
	window->m_IsRunning = false;
}

void LunaWindow::SetNewTitle(const char* newTitle)
{
	glfwSetWindowTitle(m_WindowHandle, newTitle);
}

void LunaWindow::Update()
{
	glfwSwapBuffers(m_WindowHandle);

	glfwPollEvents();
}

