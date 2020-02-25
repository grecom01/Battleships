#include "DeploymentPhase.h"
#include "Network/ServerConnection.h"
#include "Grid.h"
#include "Fleet.h"
#include "GameUI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

DeploymentPhase::DeploymentPhase(ServerConnection* server, const Grid* grid, Fleet* fleet, GameUI* gameUi)
	: m_server(server),
	  m_grid(grid),
	  m_fleet(fleet),
	  m_gameUi(gameUi),
	  m_mouseOverGrid(false)
{
}

void DeploymentPhase::init()
{
	using namespace std::placeholders;

	m_server->setFirstMoveCallback(
		std::bind(&DeploymentPhase::onFirstMoveMessage, this, _1, _2)
	);
}

void DeploymentPhase::onEnter()
{
	m_gameUi->setPlayerNameLabel(m_server->getPlayerName());
	m_gameUi->enableAwaitingEnemyLabel(false);
	m_gameUi->enableTurnLabel(false);
	m_gameUi->enableEnemyNameLabel(false);
	m_gameUi->enableEnemyShipsLeftLabel(false);
}

void DeploymentPhase::onExit()
{
	m_gameUi->enableAwaitingEnemyLabel(false);
	m_gameUi->enableTurnLabel(true);
	m_gameUi->enableEnemyNameLabel(true);
	m_gameUi->enableEnemyShipsLeftLabel(true);
}

void DeploymentPhase::onEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left)
			onMouseClickLeft(event.mouseButton);
		else if (event.mouseButton.button == sf::Mouse::Right) {
			onMouseClickRight(event.mouseButton);
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		onMouseMove(event.mouseMove);
	}
}

void DeploymentPhase::onMouseClickLeft(const sf::Event::MouseButtonEvent& event)
{
	Ship* ship = m_fleet->getUndeployedShip();
	
	if (ship != nullptr && m_grid->validCell(m_grid->toCellCoords({ event.x, event.y }))) {
		if (ship->isDeployable()) {
			m_fleet->deployShip(*m_grid, *ship);

			if (m_fleet->isFullyDeployed()) {
				m_gameUi->enableDeployInfoLabel(false);
				m_gameUi->enableAwaitingEnemyLabel(true);
				m_server->sendFleet(*m_fleet);
			}
		}
	}
}

void DeploymentPhase::onMouseClickRight(const sf::Event::MouseButtonEvent& event)
{
	Ship* ship = m_fleet->getUndeployedShip();

	if (ship != nullptr && m_grid->validCell(m_grid->toCellCoords({ event.x, event.y }))) {
		ship->rotate();
		ship->switchOrigin(*m_grid);

		if (shipDeployable(*ship)) {
			ship->setDeployable(true);
			ship->setColor(sf::Color::Green);
		}
		else {
			ship->setDeployable(false);
			ship->setColor(sf::Color::Red);
		}
	}
}

void DeploymentPhase::onMouseMove(const sf::Event::MouseMoveEvent& event)
{
	Ship* ship = m_fleet->getUndeployedShip();

	if (ship != nullptr) {
		sf::Vector2i cell = m_grid->toCellCoords({ event.x, event.y });

		if (m_mouseOverGrid = m_grid->validCell(cell)) {
			sf::Vector2f shipWindowPos = m_grid->toWindowCoords(cell);
			float cellSize = m_grid->getCellSize();

			ship->switchOrigin(*m_grid);
			ship->setPosition(shipWindowPos + sf::Vector2f(cellSize / 2, cellSize / 2));

			ship->setGridPosition(cell);
		}

		if (shipDeployable(*ship)) {
			ship->setDeployable(true);
			ship->setColor(sf::Color::Green);
		}
		else {
			ship->setDeployable(false);
			ship->setColor(sf::Color::Red);
		}
	}
}

void DeploymentPhase::onFirstMoveMessage(bool firstMove, const std::string& enemyName)
{
	m_gameUi->setEnemyNameLabel(enemyName);

	if (firstMove) {
		transition("playerMove");
	}
	else {
		transition("enemyMove");
	}
}

bool DeploymentPhase::shipDeployable(const Ship& ship) const
{
	sf::Vector2i offset = ship.isVertical() ? sf::Vector2i(0, 1) : sf::Vector2i(1, 0);
	sf::Vector2i gridPos = ship.getGridPosition();

	for (int i = 0; i < ship.getLength(); ++i) {
		sf::Vector2i cell = gridPos + i * offset;

		if (!m_grid->validCell(cell) || m_fleet->shipOnGridCell(*m_grid, cell)) {
			return false;
		}
	}

	return true;
}


void DeploymentPhase::draw()
{
	m_window->draw(*m_fleet);
	m_window->draw(*m_grid);

	Ship* ship = m_fleet->getUndeployedShip();

	if (ship != nullptr && m_mouseOverGrid) {
		m_window->draw(*ship);
	}
}