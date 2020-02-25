#include "ServerMessageQueue.h"

void ServerMessageQueue::enqueueMessage(std::function<void()> message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push(message);
}

void ServerMessageQueue::processMessages()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	while (!m_queue.empty()) {
		m_queue.front()();
		m_queue.pop();
	}
}