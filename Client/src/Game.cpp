#include "Game.h"
#include "Network/ServerConnection.h"
#include "DeploymentPhase.h"
#include "PlayerMovePhase.h"
#include "EnemyMovePhase.h"
#include "GameOverPhase.h"

Game::Game(ServerConnection* server)
	: m_server(server),
	  m_playerGrid(32.0f, 64.0f, 10, 10, 32.0f),
	  m_enemyGrid(374.0f, 64.0f, 10, 10, 32.0f),
	  m_fleet(5, m_playerGrid),
	  m_gameUi(m_fleet.getTotalSize())
{
}

void Game::init()
{
	m_fleet.createShips(2, 2);
	m_fleet.createShips(3, 2);
	m_fleet.createShips(4, 1);

	m_gameUi.build(m_window, m_server, this);

	m_gamePhases.addState<DeploymentPhase>("deploy", m_window, m_server, &m_playerGrid, &m_fleet, &m_gameUi);
	m_gamePhases.addState<PlayerMovePhase>("playerMove", m_window, m_server, &m_playerGrid, &m_enemyGrid, &m_fleet, &m_gameUi);
	m_gamePhases.addState<EnemyMovePhase>("enemyMove", m_window, m_server, &m_playerGrid, &m_enemyGrid, &m_fleet, &m_gameUi);
	m_gamePhases.addState<GameOverPhase>("gameOver", m_window, m_server, &m_playerGrid, &m_enemyGrid, &m_fleet, &m_gameUi);
}

void Game::onEnter()
{
	m_gamePhases.transition("deploy");
}

void Game::onEvent(const sf::Event& event)
{
	m_gamePhases.getCurrentState().onEvent(event);
	m_gameUi.handleEvent(event);
}

void Game::draw()
{	
	m_gamePhases.getCurrentState().draw();
	m_gameUi.draw();
}

void Game::reset()
{
	m_playerGrid.resetCells();
	m_enemyGrid.resetCells();
	m_fleet.resetShips();
	m_gameUi.resetEnemyShipsLeft();

	m_server->sendPlayAgain();
	m_gamePhases.reset("playerMove");
	m_gamePhases.transition("deploy");
}
