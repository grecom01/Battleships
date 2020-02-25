#include "EnemyMovePhase.h"
#include "Network/ServerConnection.h"
#include "Grid.h"
#include "Fleet.h"
#include "GameUI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

EnemyMovePhase::EnemyMovePhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi)
	: m_server(server),
	  m_playerGrid(playerGrid),
	  m_enemyGrid(enemyGrid),
	  m_fleet(fleet),
	  m_gameUi(gameUi)
{
}

void EnemyMovePhase::init()
{
	using namespace std::placeholders;

	m_server->setEnemyAttackCallback(
		std::bind(&EnemyMovePhase::onEnemyAttackMessage, this, _1, _2, _3)
	);
}

void EnemyMovePhase::onEnter()
{
	m_gameUi->setTurnLabel(false);
}

void EnemyMovePhase::draw()
{
	m_window->draw(*m_enemyGrid);
	m_window->draw(*m_fleet);
	m_window->draw(*m_playerGrid);
}

void EnemyMovePhase::onEnemyAttackMessage(sf::Vector2i attackCoords, bool hit, bool shipSunk)
{ 
	std::cout << "Enemy attacked (" << attackCoords.x << "," << attackCoords.y << ") and ";

	if (hit) {
		std::cout << "HIT" << std::endl;
		m_playerGrid->setCellColor(attackCoords, sf::Color::Red);
	}
	else {
		std::cout << "MISSED" << std::endl;
		m_playerGrid->setCellColor(attackCoords, sf::Color::Blue);
	}

	transition("playerMove");
}