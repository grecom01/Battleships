#pragma once

#include <queue>
#include <functional>
#include <mutex>

class ServerMessageQueue
{
	public:
	void enqueueMessage(std::function<void()> message);
	void processMessages();

	private:
	std::queue<std::function<void()>> m_queue;
	std::mutex m_mutex;
};