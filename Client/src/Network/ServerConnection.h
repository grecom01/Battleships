#pragma once

#include "MessageType.h"

#include <SFML/System/Thread.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

class Fleet;
class ServerMessageQueue;

class ServerConnection
{
	public:
	ServerConnection(ServerMessageQueue* messageQueue);
	~ServerConnection();

	void connect();
	void disconnect();
	void setConnectionEstablishedCallback(std::function<void(bool)> callback);
	void setLoginResultCallback(std::function<void(bool)> callback);
	void setFirstMoveCallback(std::function<void(bool, const std::string&)> callback);
	void setAttackResultCallback(std::function<void(sf::Vector2i, bool, bool)> callback);
	void setEnemyAttackCallback(std::function<void(sf::Vector2i, bool, bool)> callback);
	void setGameOverCallback(std::function<void(bool, bool)> callback);
	void sendLoginRequest(const std::string& name);
	void sendFleet(const Fleet& fleet);
	void sendAttack(sf::Vector2i attackCoords);
	void sendPlayAgain();

	bool isConnected() const;
	const std::string& getPlayerName() const;
	const std::string& getEnemyName() const;

	private:
	void send();
	void receive();
	void processLoginResultMessage();
	void processFirstMoveMessage();
	void processAttackResultMessage();
	void processEnemyAttackMessage();
	void processGameOverMessage();

	private:
	bool m_connected;
	bool m_sendReady;
	sf::Thread m_receiveThread;
	sf::Thread m_sendThread;
	ServerMessageQueue* m_messageQueue;
	std::mutex m_sendMutex;
	std::condition_variable m_sendCondition;
	sf::TcpSocket m_socket;
	sf::Packet m_receivePacket;
	sf::Packet m_sendPacket;
	std::string m_playerName;
	std::string m_enemyName;
	std::function<void(bool)> m_connectionEstablishedCallback;
	std::function<void(bool)> m_loginResultCallback;
	std::function<void(bool, const std::string&)> m_firstMoveCallback;
	std::function<void(sf::Vector2i, bool, bool)> m_attackResultCallback;
	std::function<void(sf::Vector2i, bool, bool)> m_enemyAttackCallback;
	std::function<void(bool, bool)> m_gameOverCallback;
};