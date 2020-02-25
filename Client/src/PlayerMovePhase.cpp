#include "PlayerMovePhase.h"
#include "Grid.h"
#include "Fleet.h"
#include "Network/ServerConnection.h"
#include "GameUI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

PlayerMovePhase::PlayerMovePhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi)
	: m_attacked(enemyGrid->getDimensions().x * enemyGrid->getDimensions().y, false),
	  m_server(server),
	  m_playerGrid(playerGrid),
	  m_enemyGrid(enemyGrid),
	  m_fleet(fleet),
	  m_gameUi(gameUi)
{
}

PlayerMovePhase::~PlayerMovePhase()
{
}

void PlayerMovePhase::init()
{
	using namespace std::placeholders;

	m_server->setAttackResultCallback(
		std::bind(&PlayerMovePhase::onAttackResultMessage, this, _1, _2, _3)
	);
}

void PlayerMovePhase::onEnter()
{
	m_gameUi->setTurnLabel(true);
}

void PlayerMovePhase::onEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			onMouseClickLeft(event.mouseButton);
		}
	}
}

void PlayerMovePhase::reset()
{
	for (size_t i = 0; i < m_attacked.size(); ++i) {
		m_attacked[i] = false;
	}
}

void PlayerMovePhase::draw()
{
	m_window->draw(*m_fleet);
	m_window->draw(*m_playerGrid);

	std::lock_guard<std::mutex> lock(m_enemyGridMutex);
	m_window->draw(*m_enemyGrid);
}

void PlayerMovePhase::onMouseClickLeft(const sf::Event::MouseButtonEvent& event)
{
	sf::Vector2i cell = m_enemyGrid->toCellCoords({ event.x, event.y });

	if (m_enemyGrid->validCell(cell) && !hasAttacked(cell)) {
		m_server->sendAttack(cell);
		setAttacked(cell, true);
	}
}

void PlayerMovePhase::onAttackResultMessage(sf::Vector2i attackCoords, bool hit, bool shipSunk)
{
	std::cout << "You attacked (" << attackCoords.x << "," << attackCoords.y << ") and ";

	if (hit) {
		std::cout << "HIT" << std::endl;

		if (shipSunk) {
			m_gameUi->updateEnemyShipsLeftLabel();
		}

		std::lock_guard<std::mutex> lock(m_enemyGridMutex);
		m_enemyGrid->setCellColor(attackCoords, sf::Color::Red);
	}
	else {
		std::cout << "MISSED" << std::endl;
		std::lock_guard<std::mutex> lock(m_enemyGridMutex);
		m_enemyGrid->setCellColor(attackCoords, sf::Color::Blue);
	}

	transition("enemyMove");
}

void PlayerMovePhase::setAttacked(sf::Vector2i cell, bool attacked)
{
	m_attacked[m_enemyGrid->getDimensions().x * cell.y + cell.x] = attacked;
}

bool PlayerMovePhase::hasAttacked(sf::Vector2i cell) const
{
	return m_attacked[m_enemyGrid->getDimensions().x * cell.y + cell.x];
}