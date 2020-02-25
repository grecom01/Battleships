#include "GameOverPhase.h"
#include "Grid.h"
#include "Fleet.h"
#include "Network/ServerConnection.h"
#include "GameUI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

GameOverPhase::GameOverPhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi)
	: m_server(server),
	  m_playerGrid(playerGrid),
	  m_enemyGrid(enemyGrid),
	  m_fleet(fleet),
	  m_gameUi(gameUi)
{
}

void GameOverPhase::init()
{
	using namespace std::placeholders;

	m_server->setGameOverCallback(std::bind(&GameOverPhase::onGameOverMessage, this, _1, _2));
}

void GameOverPhase::onEnter()
{
	m_gameUi->enableGameOverPanel(true);
}

void GameOverPhase::onExit()
{
	m_gameUi->enableGameOverPanel(false);
}

void GameOverPhase::draw()
{
	m_window->draw(*m_fleet);
	m_window->draw(*m_playerGrid);
	m_window->draw(*m_enemyGrid);
}

void GameOverPhase::onGameOverMessage(bool victorious, bool fleetDestroyed)
{
	m_gameUi->setupGameOverPanel(victorious, fleetDestroyed);
	transition("gameOver");
}

