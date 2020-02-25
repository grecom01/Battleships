#pragma once

#include "State.h"
#include "LoginUI.h"

#include <future>

class ServerConnection;

class Login : public State
{
	public:
	Login(ServerConnection* server);

	virtual void init() override;
	virtual void onEvent(const sf::Event& event) override;
	virtual void draw() override;

	private:
	void onConnectionEstablishedMessage(bool established);
	void onLoginResultMessage(bool success);

	private:
	ServerConnection* m_server;
	LoginUI m_loginUi;
	std::future<void> m_connectFuture;
};