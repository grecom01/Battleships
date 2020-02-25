#pragma once

#include "State.h"

#include <mutex>
#include <vector>

class Grid;
class Fleet;
class ServerConnection;
class GameUI;

class PlayerMovePhase : public State
{
	public:
	PlayerMovePhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi);
	~PlayerMovePhase();

	virtual void init() override;
	virtual void onEnter() override;
	virtual void onEvent(const sf::Event& event) override;
	virtual void reset() override;
	virtual void draw() override;

	private:
	void onMouseClickLeft(const sf::Event::MouseButtonEvent& event);
	void onAttackResultMessage(sf::Vector2i attackCoords, bool hit, bool shipSunk);
	void setAttacked(sf::Vector2i cell, bool attacked);

	bool hasAttacked(sf::Vector2i cell) const;

	private:
	std::vector<bool> m_attacked;
	ServerConnection* m_server;
	Grid* m_playerGrid;
	Grid* m_enemyGrid;
	Fleet* m_fleet;
	GameUI* m_gameUi;
	std::mutex m_enemyGridMutex;
};