// Luna Engine - Luna Window

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

/*
Responsible for managing the window of an application
Currently only tested with one concurrent window, however in theory supports multi-windowed application
 - Open LunaWindow.cpp to learn more -
*/
class LunaWindow
{
public:
	LunaWindow();
	~LunaWindow();
	
	// Stores a reference to the currently focused window for global access
	static LunaWindow* m_FocusedWindow;

	// Returns m_IsRunning
	bool IsRunning() {return m_IsRunning;}

	// Returns the state of int key. reference GLFW input documentation for more details
	int GetKey(int key);
	// Returns the state of int button. reference GLFW input documentation for more details
	int GetMouseButton(int button);
	// Stores the current cursor position into double* x and double* y accordingly
	void GetCursorPosition(double* x, double* y);

	// Overrides the current cursor position using double x and double y accordingly
	void SetCursorPosition(double x, double y);
	// Sets the GLFW input mode of the window. reference glfwSetInputMode documentation for more details
	void SetInputMode(int mode, int value);

	// Sets m_FocusedWindow to this window instance
	void FocusWindow();

	// Returns a reference to a newly created window instance
	// const char* windowTitle - the initialized window title
	// uint32_t windowWidth, windowHeight - the initialized window width and height
	static LunaWindow* NewWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight);
	// Closes the window instance
	// LunaWindow* window - the window that should be closed
	static void CloseWindow(LunaWindow* window);
	// Returns whether this instance of LunaWindow should close
	bool ShouldClose();

	// Sets the title of the current window
	// const char* newTitle - the data that should be passed to the new window title
	void SetNewTitle(const char* newTitle);

	// Returns the window handle
	GLFWwindow* GetHandle(){return m_WindowHandle;}

	// Swaps the window buffers - should be called at the end of every frame
	void Update();
private:
	GLFWwindow* m_WindowHandle = nullptr;
	bool m_IsRunning = false;

	const char* m_WindowTitle;
	uint32_t m_WindowWidth, m_WindowHeight;
};