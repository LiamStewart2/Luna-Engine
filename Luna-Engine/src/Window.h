#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	~Window();

	static Window* CreateWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight);
	static Window* DestroyWindow(Window* window);

	void Update();

private:
	GLFWwindow* window;

	const char* m_WindowTitle;
	uint32_t m_WindowWidth, m_WindowHeight;
};