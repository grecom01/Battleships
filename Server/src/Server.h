#pragma once

#include "Player.h"

#include <SFML/Network.hpp>
#include <unordered_map>
#include <list>

class Server
{
	public:
	Server(unsigned int port);
	
	void run();

	private:
	bool startListening();
	void acceptClient();
	void removeDisconnectedPlayers();
	void startNewGames();

	private:
	unsigned int m_port;
	bool m_running;
	sf::TcpListener m_listener;
	sf::SocketSelector m_selector;
	std::list<Player> m_players;
};