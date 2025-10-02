// Luna Engine - Luna Window

/*
Responsible for managing the window of an application
Currently only tested with one concurrent window, however in theory supports multi-windowed application

 -- Creating and Managing a Singular window --
- Creating a Window
 To create a new Luna Window, call LunaWindow::NewWindow() which returns a pointer to a window stored on the heap
 Currently the only customizability to windows is the title and size, however in future more features may be added
 
 - Creating a mainloop
 Create a loop using:
 while(!window->ShouldClose()) {} where window is a LunaWindow reference
 At the end of every frame, call window->Update();
 At the end of the loop, call LunaWindow::CloseWindow(window);

 - Misc
 Use the API's wrapper functions to check input, close the window and other misc functions
 Any functions not yet provided by the API that GLFW provides can be accessed by called GLFWwindow* GetHandle()
 If any contributors want to implement these functions feel free
 */

#include "LunaWindow.h"

// Stores a reference to the currently focused window for global access
LunaWindow* LunaWindow::m_FocusedWindow = nullptr;

LunaWindow::LunaWindow()
{
	m_WindowTitle = "";
	m_WindowWidth = m_WindowHeight = 0;
}

LunaWindow::~LunaWindow()
{

}

// Returns the state of int key. reference GLFW input documentation for more details
int LunaWindow::GetKey(int key)
{
	return glfwGetKey(m_WindowHandle, key);
}

// Returns the state of int button. reference GLFW input documentation for more details
int LunaWindow::GetMouseButton(int button)
{
	return glfwGetMouseButton(m_WindowHandle, button);
}

// Stores the current cursor position into double* x and double* y accordingly
void LunaWindow::GetCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(m_WindowHandle, x, y);
}

// Overrides the current cursor position using double x and double y accordingly
void LunaWindow::SetCursorPosition(double x, double y)
{
	glfwSetCursorPos(m_WindowHandle, x, y);
}

// Sets the GLFW input mode of the window. reference glfwSetInputMode documentation for more details
void LunaWindow::SetInputMode(int mode, int value)
{
	glfwSetInputMode(m_WindowHandle, mode, value);
}

// Returns a reference to a newly created window instance
// const char* windowTitle - the initialized window title
// uint32_t windowWidth, windowHeight - the initialized window width and height
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
	glfwWindowHint(GLFW_NO_API, GLFW_TRUE);

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

	// Only set is running to true if all phases of init pass
	window->m_IsRunning = true;
	LunaWindow::m_FocusedWindow = window;

	return window;
}

// Sets m_FocusedWindow to this window instance
void LunaWindow::FocusWindow()
{
	LunaWindow::m_FocusedWindow = this;
}

// Closes the window instance
// LunaWindow* window - the window that should be closed
void LunaWindow::CloseWindow(LunaWindow* window)
{
	glfwWindowShouldClose(window->m_WindowHandle);
	glfwTerminate();
	window->m_IsRunning = false;
}

// Returns whether this instance of LunaWindow should close
bool LunaWindow::ShouldClose()
{
	return glfwWindowShouldClose(m_WindowHandle);
}

// Sets the title of the current window
// const char* newTitle - the data that should be passed to the new window title
void LunaWindow::SetNewTitle(const char* newTitle)
{
	glfwSetWindowTitle(m_WindowHandle, newTitle);
}

// Polls window events - should be called at the end of every frame
void LunaWindow::Update()
{
	glfwPollEvents();
}