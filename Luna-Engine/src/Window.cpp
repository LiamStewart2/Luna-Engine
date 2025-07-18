#include "Window.h"

Window::Window()
{
	m_WindowTitle = "";
	m_WindowWidth = m_WindowHeight = 0;
}

Window::~Window()
{

}

int Window::GetKey(int key)
{
	return glfwGetKey(m_WindowHandle, key);
}

int Window::GetMouseButton(int button)
{
	return glfwGetMouseButton(m_WindowHandle, button);
}

void Window::GetCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(m_WindowHandle, x, y);
}

void Window::SetCursorPosition(double x, double y)
{
	glfwSetCursorPos(m_WindowHandle, x, y);
}

void Window::SetInputMode(int mode, int value)
{
	glfwSetInputMode(m_WindowHandle, mode, value);
}

Window* Window::CreateWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight)
{
	// Test GLFW init
	if (!glfwInit())
	{
		std::cerr << "GLFW FAILED INIT" << std::endl;
		return nullptr;
	}

	// Initialize window class instance with member variables
	Window* window = new Window();

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

	return window;
}

void Window::CloseWindow(Window* window)
{
	glfwWindowShouldClose(window->m_WindowHandle);
	glfwTerminate();
	window->m_IsRunning = false;
}

void Window::SetNewTitle(const char* newTitle)
{
	glfwSetWindowTitle(m_WindowHandle, newTitle);
}

void Window::Update()
{
	glfwSwapBuffers(m_WindowHandle);

	glfwPollEvents();
}

