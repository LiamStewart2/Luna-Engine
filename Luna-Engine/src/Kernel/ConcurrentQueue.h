#pragma once

#include <mutex>

template <class T>
struct ConcurrentQueueNode
{
	T m_Data;
	ConcurrentQueueNode<T>* m_Next = nullptr;
};

template <class T>
class ConcurrentQueue
{
public:
	ConcurrentQueue()
	{
		ConcurrentQueueNode<T>* dummyNode = new ConcurrentQueueNode<T>();
		m_FrontNode = m_BackNode = dummyNode;
	}

	void Push(T object)
	{
		ConcurrentQueueNode<T>* nextNode = new ConcurrentQueueNode<T>();
		nextNode->m_Data = object;

		m_BackLock.lock();

		m_BackNode->m_Next = nextNode;
		m_BackNode = nextNode;

		m_BackLock.unlock();
	}

	bool Pop(T& object)
	{
		m_FrontLock.lock();

		ConcurrentQueueNode<T>* frontNode = m_FrontNode;
		ConcurrentQueueNode<T>* newFrontNode = frontNode->m_Next;

		if (newFrontNode == nullptr)
		{
			m_FrontLock.unlock();
			return false;
		}

		object = newFrontNode->m_Data;
		m_FrontNode = newFrontNode;

		m_FrontLock.unlock();
		delete frontNode;
		return true;
	}

private:
	std::mutex m_FrontLock;
	std::mutex m_BackLock;

	ConcurrentQueueNode<T>* m_FrontNode = nullptr;
	ConcurrentQueueNode<T>* m_BackNode = nullptr;
};