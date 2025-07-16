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

	void ResetStack()
	{
		m_Top = 0;
	}

	void Push(T value)
	{
		if(m_Top >= m_Size)
			std::cerr << "STACK EXCEEDS STACK SIZE OF " << m_Size << std::endl;
		m_StackArray[m_Top++] = value;
	}
	T Pop()
	{
		return m_StackArray[--m_Top];
	}

	T Peek()
	{
		return m_StackArray[m_Top - 1];
	}
private:
	int m_Size;
	int m_Top;
	T* m_StackArray;
};