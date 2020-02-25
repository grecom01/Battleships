#include "ServerConnection.h"
#include "Fleet.h"
#include "ServerMessageQueue.h"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <iostream>

ServerConnection::ServerConnection(ServerMessageQueue* messageQueue)
	: m_connected(false),
	  m_sendReady(false),
	  m_receiveThread(&ServerConnection::receive, this),
	  m_sendThread(&ServerConnection::send, this),
	  m_messageQueue(messageQueue)
{
}

ServerConnection::~ServerConnection()
{
	m_receiveThread.wait();
	m_sendThread.wait();
}

void ServerConnection::connect()
{
	sf::Socket::Status status = m_socket.connect(sf::IpAddress::getLocalAddress(), 33333, sf::seconds(10.0f));

	if (m_connected = status == sf::Socket::Done) {
		m_socket.setBlocking(false);

		m_receiveThread.launch();
		m_sendThread.launch();

		std::cout << "Connected to server." << std::endl;
	}
	else {
		std::cout << "Could not connect to server!" << std::endl;
	}

	m_messageQueue->enqueueMessage([&] {
		m_connectionEstablishedCallback(m_connected);
	});
}

void ServerConnection::disconnect()
{
	m_connected = false;
	m_socket.disconnect();
	m_sendCondition.notify_one();
}
void ServerConnection::setConnectionEstablishedCallback(std::function<void(bool)> callback)
{
	m_connectionEstablishedCallback = callback;
}

void ServerConnection::setLoginResultCallback(std::function<void(bool)> callback)
{
	m_loginResultCallback = callback;
}

void ServerConnection::setFirstMoveCallback(std::function<void(bool, const std::string&)> callback)
{
	m_firstMoveCallback = callback;
}

void ServerConnection::setAttackResultCallback(std::function<void(sf::Vector2i, bool, bool)> callback)
{
	m_attackResultCallback = callback;
}

void ServerConnection::setEnemyAttackCallback(std::function<void(sf::Vector2i, bool, bool)> callback)
{
	m_enemyAttackCallback = callback;
}

void ServerConnection::setGameOverCallback(std::function<void(bool, bool)> callback)
{
	m_gameOverCallback = callback;
}

void ServerConnection::sendLoginRequest(const std::string& name)
{
	m_playerName = name;

	std::unique_lock<std::mutex> lock(m_sendMutex);
	m_sendPacket << (sf::Int8)ClientMessageType::Login << name;
	m_sendReady = true;
	m_sendCondition.notify_one();
}

void ServerConnection::sendFleet(const Fleet& fleet)
{
	std::unique_lock<std::mutex> lock(m_sendMutex);
	m_sendPacket << (sf::Int8)ClientMessageType::Fleet << fleet;
	m_sendReady = true;
	m_sendCondition.notify_one();
}

void ServerConnection::sendAttack(sf::Vector2i attackCoords)
{
	std::unique_lock<std::mutex> lock(m_sendMutex);
	m_sendPacket << (sf::Int8)ClientMessageType::Attack
		         << (sf::Int32)attackCoords.x << (sf::Int32)attackCoords.y;

	m_sendReady = true;
	m_sendCondition.notify_one();
}

void ServerConnection::sendPlayAgain()
{
	std::unique_lock<std::mutex> lock(m_sendMutex);
	m_sendPacket << (sf::Int8)ClientMessageType::PlayAgain;
	m_sendReady = true;
	m_sendCondition.notify_one();
}

bool ServerConnection::isConnected() const
{
	return m_connected;
}

const std::string& ServerConnection::getPlayerName() const
{
	return m_playerName;
}

const std::string& ServerConnection::getEnemyName() const
{
	return m_enemyName;
}

void ServerConnection::send()
{
	while (true) {
		std::unique_lock<std::mutex> lock(m_sendMutex);

		while (!m_sendReady) {
			m_sendCondition.wait(lock);

			if (!m_connected) {
				return;
			}
		}

		std::cout << "Sending Packet of size: " << m_sendPacket.getDataSize() << std::endl;
		while (m_socket.send(m_sendPacket) == sf::Socket::Partial);
		m_sendReady = false;
		m_sendPacket.clear();
	}
}

void ServerConnection::receive()
{
	while (m_connected) {
		sf::Socket::Status receiveStatus = m_socket.receive(m_receivePacket);

		if (receiveStatus == sf::Socket::Done) {
			sf::Int8 messageType = -1;
			m_receivePacket >> messageType;

			switch ((ServerMessageType)messageType) {
				case ServerMessageType::LoginResult:
					processLoginResultMessage();
					break;

				case ServerMessageType::FirstMove:
					processFirstMoveMessage();
					break;

				case ServerMessageType::AttackResult:
					processAttackResultMessage();
					break;

				case ServerMessageType::EnemyAttack:
					processEnemyAttackMessage();
					break;

				case ServerMessageType::GameOver:
					processGameOverMessage();
					break;

				default:
					std::cout << "Received invalid message" << std::endl;
					break;
			}
		}
		else if (receiveStatus == sf::Socket::Error) {
			std::cout << "Error: Could not receive packet" << std::endl;
			// TODO: handle server termination
		}
	}
}

void ServerConnection::processLoginResultMessage()
{
	bool result = false;
	m_receivePacket >> result;
	m_receivePacket.clear();

	m_messageQueue->enqueueMessage([&, result] {
		m_loginResultCallback(result);
	});
}

void ServerConnection::processFirstMoveMessage()
{
	bool firstMove = false;
	m_receivePacket >> firstMove >> m_enemyName;
	m_receivePacket.clear();

	m_messageQueue->enqueueMessage([&, firstMove] {
		m_firstMoveCallback(firstMove, m_enemyName);
	});
}

void ServerConnection::processAttackResultMessage()
{
	sf::Vector2i attackCoords;
	bool attackResult = false;
	bool shipSunk = false;

	m_receivePacket >> attackCoords.x >> attackCoords.y
		            >> attackResult >> shipSunk;

	m_receivePacket.clear();

	m_messageQueue->enqueueMessage([&, attackCoords, attackResult, shipSunk] {
		m_attackResultCallback(attackCoords, attackResult, shipSunk);
	});
}

void ServerConnection::processEnemyAttackMessage()
{
	sf::Vector2i attackCoords;
	bool attackResult = false;
	bool shipSunk = false;

	m_receivePacket >> attackCoords.x >> attackCoords.y
		>> attackResult >> shipSunk;

	m_receivePacket.clear();

	m_messageQueue->enqueueMessage([&, attackCoords, attackResult, shipSunk] {
		m_enemyAttackCallback(attackCoords, attackResult, shipSunk);
	});
}

void ServerConnection::processGameOverMessage()
{
	bool victorious = false;
	bool enemyDestroyed = false;

	m_receivePacket >> victorious >> enemyDestroyed;
	m_receivePacket.clear();

	m_messageQueue->enqueueMessage([&, victorious, enemyDestroyed] {
		m_gameOverCallback(victorious, enemyDestroyed);
	});
}
