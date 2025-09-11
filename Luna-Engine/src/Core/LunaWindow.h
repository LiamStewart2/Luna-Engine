#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class LunaWindow
{
public:
	LunaWindow();
	~LunaWindow();

	static LunaWindow* m_FocusedWindow;

	bool IsRunning() {return m_IsRunning;}

	int GetKey(int key);
	int GetMouseButton(int button);
	void GetCursorPosition(double* x, double* y);

	void SetCursorPosition(double x, double y);
	void SetInputMode(int mode, int value);

	void FocusWindow();

	static LunaWindow* NewWindow(const char* windowTitle, uint32_t windowWidth, uint32_t windowHeight);
	static void CloseWindow(LunaWindow* window);

	void SetNewTitle(const char* newTitle);
	GLFWwindow* GetHandle(){return m_WindowHandle;}
	void Update();
private:
	GLFWwindow* m_WindowHandle = nullptr;
	bool m_IsRunning = false;

	const char* m_WindowTitle;
	uint32_t m_WindowWidth, m_WindowHeight;
};