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

	static Window* CreateWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight);
	static void CloseWindow(Window* window);

	void Update();
private:
	GLFWwindow* m_WindowHandle = nullptr;
	bool m_IsRunning = false;

	const char* m_WindowTitle;
	uint32_t m_WindowWidth, m_WindowHeight;
};