#pragma once

#include <iostream>

template <class T>
struct LinkedListNode
{
	T data;
	LinkedListNode* next;
};

template <class T>
class LinkedList
{
public:
	LinkedList() {}
	~LinkedList()
	{
		LinkedListNode<T>* current = first;
		while (current != nullptr)
		{
			LinkedListNode<T>* nextNode = current->next;
			delete current;
			current = nextNode;
			size -= 1;
		}
	}

	int Size() { return size; }

	// Push element to the back of the list
	void Push(T data)
	{
		LinkedListNode<T>* element = new LinkedListNode<T>();
		element->data = data; element->next = nullptr;

		if (first == nullptr)
			first = element;
		else
			last->next = element;

		last = element;
		size += 1;
	}

	// get the nth element of the lsit
	T& operator[] (int index)
	{
		if (index > size)
		{
			std::cerr << "index out of range" << std::endl;
			throw std::out_of_range("Index is out of range");
		}

		LinkedListNode<T>* current = first;
		for (int i = 0; i < index; i++)
			current = current->next;

		return current->data;
	}


private:
	LinkedListNode<T>* first = nullptr;
	LinkedListNode<T>* last = nullptr;

	int size = 0;
};