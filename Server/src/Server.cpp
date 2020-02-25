#include "Server.h"

#include <SFML/System/Clock.hpp>
#include <iostream>

Server::Server(unsigned int port)
	: m_port(port),
	  m_running(false)
{
}

void Server::run()
{
	if (!startListening()) {
		std::cout << "Shutting down server." << std::endl;
		return;
	}

	m_running = true;	
	m_selector.add(m_listener);

	while (m_running) {
		if (m_selector.wait(sf::Time(sf::seconds(5.0f)))) {
			if (m_selector.isReady(m_listener)) {
				acceptClient();
			}
			else {
				for (auto& player : m_players) {
					if (player.hasIncomingMessage()) {
						std::cout << "Receiving message from Player" << std::endl;
						player.receiveMessage();
					}
				}
			}
		}

		removeDisconnectedPlayers();
		startNewGames();

	}
}

bool Server::startListening()
{
	if (m_listener.listen(m_port) != sf::Socket::Done) {
		std::cout << "Could not bind listener to port" << std::endl;
		return false;
	}

	std::cout << "Server online ("
		<< sf::IpAddress::getLocalAddress() << ":" << m_port << ")"
		<< std::endl;

	return true;
}

void Server::acceptClient()
{
	std::unique_ptr<sf::TcpSocket> playerSocket = std::make_unique<sf::TcpSocket>();

	if (m_listener.accept(*playerSocket) == sf::Socket::Done) {
		m_players.emplace_back(std::move(playerSocket), m_selector);

		std::cout << "New connection established.\nCurrently connected: "
			      << m_players.size() << std::endl;
	}
}

void Server::removeDisconnectedPlayers()
{
	for (auto it = m_players.begin(); it != m_players.end();) {
		if (it->isDisconnected()) {
			it = m_players.erase(it);
			std::cout << "Disconnected Player removed" << std::endl;
		}
		else {
			++it;
		}
	}
}

void Server::startNewGames()
{
	Player* player1 = nullptr;
	Player* player2 = nullptr;

	for (auto& player : m_players) {
		if (player.isReadyToPlay()) {
			if (player1 == nullptr) {
				player1 = &player;
			}
			else if (player2 == nullptr) {
				player2 = &player;
			}

			if (player1 != nullptr && player2 != nullptr) {
				player1->setEnemyPlayer(*player2);
				player2->setEnemyPlayer(*player1);
				
				player1->startGame();

				player1 = nullptr;
				player2 = nullptr;
			}
		}
	}
}
