#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window
{
public:
	Window();
	~Window();

	bool IsRunning() {return m_IsRunning;}

	int GetKey(int key);
	int GetMouseButton(int button);
	void GetCursorPosition(double* x, double* y);

	void SetCursorPosition(double x, double y);
	void SetInputMode(int mode, int value);

	static Window* CreateWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight);
	static void CloseWindow(Window* window);

	void SetNewTitle(const char* newTitle);

	void Update();
private:
	GLFWwindow* m_WindowHandle = nullptr;
	bool m_IsRunning = false;

	const char* m_WindowTitle;
	uint32_t m_WindowWidth, m_WindowHeight;
};