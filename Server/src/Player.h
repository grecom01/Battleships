#pragma once

#include "Ship.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <memory>
#include <string>

class Player
{
	public:
	Player(std::unique_ptr<sf::TcpSocket>&& client, sf::SocketSelector& selector);

	void startGame();
	void receiveMessage();
	void setEnemyPlayer(Player& enemy);

	bool hasIncomingMessage() const;
	bool isReadyToPlay() const;
	bool isDisconnected() const;

	private:
	void processLoginMessage();
	void processFleetMessage();
	void processAttackMessage();
	void processPlayAgainMessage();
	void sendFirstMoveMessage(bool firstMove, const std::string& enemyName);
	void sendGameOverMessage(bool victorious, bool fleetDestroyed);

	bool isFleetDestroyed() const;

	private:
	bool m_readyToPlay;
	Player* m_enemy;
	std::unique_ptr<sf::TcpSocket> m_client;
	sf::SocketSelector* m_selector;
	sf::Packet m_packet;
	std::string m_name;
	std::vector<Ship> m_fleet;
};