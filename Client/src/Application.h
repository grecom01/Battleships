#pragma once

#include "StateManager.h"
#include "Network/ServerMessageQueue.h"
#include "Network/ServerConnection.h"

#include <SFML/Graphics.hpp>

class Application
{
	public:
	Application(const std::string& title, int windowWidth, int windowHeight);
	
	void run();

	private:
	void loadAssets();
	void initStates();

	private:
	sf::RenderWindow m_window;
	ServerMessageQueue m_messageQueue;
	ServerConnection m_server;
	StateManager m_stateManager;
};