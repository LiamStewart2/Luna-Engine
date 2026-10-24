#pragma once

#include <mutex>
#include <queue>

// ToDo - make this a lock free queue using the atomic approach
template <class T>
class ConcurrentQueue
{
public:
	ConcurrentQueue()
	{
		m_Queue = std::queue<T>();
	}

	void Push(T object)
	{
		m_Lock.lock();
		m_Queue.push(object);
		m_Lock.unlock();
	}
	T Pop()
	{
		m_Lock.lock();
		T object = m_Queue.front();
		m_Queue.pop();
		m_Lock.unlock();
		return object;
	}

	bool isEmpty()
	{
		return m_Queue.empty();
	}

private:
	std::queue<T> m_Queue;
	std::mutex m_Lock;
};