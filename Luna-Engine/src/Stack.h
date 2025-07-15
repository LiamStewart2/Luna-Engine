#pragma once

#include <iostream>

template <class T>
class Stack
{
public:
	Stack(int size)
	{
		m_Top = 0; m_Size = size;
		m_StackArray = new T[size];
	}
	~Stack()
	{
		delete[] m_StackArray;
	}

	void Push(T value)
	{
		if(m_Top >= m_Size)
			std::cerr << "STACK EXCEEDS STACK SIZE OF " << m_Size << std::endl;
		m_StackArray[m_Top] = value;
		m_Top += 1;
	}
	T Pop()
	{
		m_Top -= 1;
		return m_StackArray[m_Top + 1];
	}

	T Peek()
	{
		return m_StackArray[m_Top];
	}
private:
	int m_Size;
	int m_Top;
	T* m_StackArray;
};