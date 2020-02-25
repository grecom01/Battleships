#pragma once

#include "State.h"

class Grid;
class Fleet;
class Ship;
class ServerConnection;
class GameUI;

class DeploymentPhase : public State
{
	public:
	DeploymentPhase(ServerConnection* server, const Grid* grid, Fleet* fleet, GameUI* gameUi);

	virtual void init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onEvent(const sf::Event& event) override;
	virtual void draw() override;

	private:
	void onMouseClickLeft(const sf::Event::MouseButtonEvent& event);
	void onMouseClickRight(const sf::Event::MouseButtonEvent& event);
	void onMouseMove(const sf::Event::MouseMoveEvent& event);
	void onFirstMoveMessage(bool firstMove, const std::string& enemyName);

	bool shipDeployable(const Ship& ship) const;



	private:
	ServerConnection* m_server;
	const Grid* m_grid;
	Fleet* m_fleet;
	GameUI* m_gameUi;
	bool m_mouseOverGrid;
};