#include "Login.h"
#include "Network/ServerConnection.h"

#include <iostream>


Login::Login(ServerConnection* server)
	: m_server(server)
{
}

void Login::init()
{
	m_server->setLoginResultCallback(
		std::bind(&Login::onLoginResultMessage, this, std::placeholders::_1)
	);

	m_server->setConnectionEstablishedCallback(
		std::bind(&Login::onConnectionEstablishedMessage, this, std::placeholders::_1)
	);

	m_loginUi.build(m_window, m_server);

	m_connectFuture = std::async(std::launch::async, [&] {
		m_server->connect();
	});
}

void Login::onEvent(const sf::Event& event)
{
	m_loginUi.handleEvent(event);
}

void Login::draw()
{
	m_loginUi.draw();
}

void Login::onConnectionEstablishedMessage(bool established)
{
	m_loginUi.setConnectionStatus(established);
	m_loginUi.enableLoginForm(true);
}

void Login::onLoginResultMessage(bool success)
{
	if (success) {
		std::cout << "Login successful" << std::endl;
		transition("game");
	}
}