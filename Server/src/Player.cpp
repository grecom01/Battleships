#include "Player.h"
#include "MessageType.h"

#include <SFML/Network/SocketSelector.hpp>
#include <iostream>

Player::Player(std::unique_ptr<sf::TcpSocket>&& client, sf::SocketSelector& selector)
	: m_readyToPlay(false),
	  m_enemy(nullptr),
	  m_client(std::move(client)),
	  m_selector(&selector)
{
	m_selector->add(*m_client);
}

void Player::startGame()
{
	m_readyToPlay = false;
	m_enemy->m_readyToPlay = false;
	sendFirstMoveMessage(true, m_enemy->m_name);
	m_enemy->sendFirstMoveMessage(false, m_name);
}

void Player::receiveMessage()
{
	sf::Socket::Status receiveStatus = m_client->receive(m_packet);

	if (receiveStatus == sf::Socket::Done) {
		sf::Int8 msgType;
		m_packet >> msgType;

		switch ((ClientMessageType)msgType) {
			case ClientMessageType::Login:
				processLoginMessage();
				break;

			case ClientMessageType::Fleet:
				processFleetMessage();
				break;

			case ClientMessageType::Attack:
				processAttackMessage();

				if (m_enemy->isFleetDestroyed()) {
					sendGameOverMessage(true, true);
					m_enemy->sendGameOverMessage(false, true);
					m_enemy->m_enemy = nullptr;
					m_enemy = nullptr;
				}

				break;

			case ClientMessageType::PlayAgain:
				processPlayAgainMessage();
				break;

			default:
				std::cout << "Client send unknown message" << std::endl;
		}
	}
	else {
		if (m_enemy != nullptr) {
			m_enemy->sendGameOverMessage(true, false);
			m_enemy->m_enemy = nullptr;
		}

		std::cout << "Lost connection to client" << std::endl;
		m_selector->remove(*m_client);
		m_client->disconnect();
	}
}
void Player::setEnemyPlayer(Player& enemy)
{
	m_enemy = &enemy;
}

bool Player::hasIncomingMessage() const
{
	return m_selector->isReady(*m_client);
}

bool Player::isReadyToPlay() const
{
	return m_readyToPlay;
}

bool Player::isDisconnected() const
{
	return m_client->getLocalPort() == 0;
}

void Player::processLoginMessage()
{
	m_packet >> m_name;

	m_packet.clear();
	m_packet << (sf::Int8)ServerMessageType::LoginResult << true;
	m_client->send(m_packet);
	m_packet.clear();

	std::cout << "Player " << m_name << " logged in successfully" << std::endl;
}

void Player::processFleetMessage()
{
	sf::Uint8 fleetSize = 0;
	m_packet >> fleetSize;

	m_fleet.clear();
	m_fleet.reserve((size_t)fleetSize);

	for (sf::Uint8 i = 0; i < fleetSize; ++i) {
		m_fleet.emplace_back();
		m_packet >> m_fleet[i];
		//m_fleet[i].printStatus();
	}

	m_readyToPlay = true;
	m_packet.clear();
}

void Player::processAttackMessage()
{
	sf::Vector2i attackCoords;
	m_packet >> attackCoords.x >> attackCoords.y;

	bool attackResult = false;
	bool shipSunk = false;

	for (size_t i = 0; i < m_enemy->m_fleet.size(); ++i) {
		attackResult = m_enemy->m_fleet[i].takeHit(attackCoords);
		shipSunk = m_enemy->m_fleet[i].hasSunk();

		if (attackResult) {
			break;
		}
	}

	m_packet.clear();

	m_packet << (sf::Int8)ServerMessageType::AttackResult
			 << (sf::Int32)attackCoords.x << (sf::Int32)attackCoords.y
			 << attackResult << shipSunk;
			   
	m_client->send(m_packet);

	m_packet.clear();
	m_packet << (sf::Int8)ServerMessageType::EnemyAttack
		     << (sf::Int32)attackCoords.x << (sf::Int32)attackCoords.y
		     << attackResult << shipSunk;

	m_enemy->m_client->send(m_packet);
	m_packet.clear();
}

void Player::processPlayAgainMessage()
{
	m_fleet.clear();
	m_packet.clear();
}

void Player::sendFirstMoveMessage(bool firstMove, const std::string& enemyName)
{
	m_packet << (sf::Int8)ServerMessageType::FirstMove << firstMove << enemyName;
	m_client->send(m_packet);
	m_packet.clear();
}

void Player::sendGameOverMessage(bool victorious, bool fleetDestroyed)
{
	m_packet << (sf::Int8)ServerMessageType::GameOver << victorious << fleetDestroyed;
	m_client->send(m_packet);
	m_packet.clear();
}

bool Player::isFleetDestroyed() const
{
	for (const auto& ship : m_fleet) {
		if (!ship.hasSunk()) {
			return false;
		}
	}

	return true;
}