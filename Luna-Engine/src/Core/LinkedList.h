// Luna Engine - Linked List

#pragma once

#include <iostream>

/*
The node class used to store the data of each node of type T
*/
template <class T>
struct LinkedListNode
{
	T data;
	LinkedListNode* next;
};


/*
Basic Linked List implementation to support storage of data that shouldn't move in memory
class T - the data type wanting to be stored in the Linked List
*/
template <class T>
class LinkedList
{
public:
	LinkedList() {}
	// Iterativly deletes all nodes in the list
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

	// Returns the number of elements in the list
	int Size() { return size; }

	// Pushes element to the back of the list
	// T data - the object you want storing in the list node
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

	// Returns the n'th element of the list
	// int index - the index of the node wanting to be accessed
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