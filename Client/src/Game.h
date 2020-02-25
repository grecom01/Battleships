#pragma once

#include "StateManager.h"
#include "Grid.h"
#include "Fleet.h"
#include "GameUI.h"

class ServerConnection;

class Game : public State
{
	public:
	Game(ServerConnection* server);

	virtual void init() override;
	virtual void onEnter() override;
	virtual void onEvent(const sf::Event& event) override;
	virtual void draw() override;

	void reset();

	private:
	ServerConnection* m_server;
	Grid m_playerGrid;
	Grid m_enemyGrid;
	Fleet m_fleet;
	GameUI m_gameUi;
	StateManager m_gamePhases;
};