// Luna Engine - Stack

#pragma once

#include <iostream>


/*
Basic Stack implementation to support stack data structures of any data type
class T - the data type wanting to be stored in the stack
*/
template <class T>
class Stack
{
public:
	// Create a stack of capacity size - cannot dynamically expand
	// int size - the maximum capacity of the stack's internal array
	Stack(int size)
	{
		m_Top = 0; m_Size = size;
		m_StackArray = new T[size];
	}
	// Cleans up the internal array
	~Stack()
	{
		delete[] m_StackArray;
	}

	// Reset the stack to the first node in the list
	// Does not clear the memory in the stack array
	void ResetStack()
	{
		m_Top = 0;
	}

	// Pushes element to the top of the stack
	// T value - the value wanting to be stored in the stack
	void Push(T value)
	{
		if(m_Top >= m_Size)
			std::cerr << "STACK EXCEEDS STACK SIZE OF " << m_Size << std::endl;
		m_StackArray[m_Top++] = value;
	}
	
	// Returns the top of the stack and sets the top to the element below
	T Pop()
	{
		return m_StackArray[--m_Top];
	}

	// Returns the top of the stack whilst making no change to the top of the stack
	T Peek()
	{
		return m_StackArray[m_Top - 1];
	}
private:
	int m_Size;
	int m_Top;
	T* m_StackArray;
};