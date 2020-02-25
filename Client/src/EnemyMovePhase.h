#pragma once

#include "State.h"

#include <mutex>

class Grid;
class Fleet;
class ServerConnection;
class GameUI;

class EnemyMovePhase : public State
{
	public:
	EnemyMovePhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi);

	virtual void init() override;
	virtual void onEnter() override;
	virtual void draw() override;

	private:
	void onEnemyAttackMessage(sf::Vector2i attackCoords, bool hit, bool shipSunk);

	private:
	ServerConnection* m_server;
	Grid* m_playerGrid;
	Grid* m_enemyGrid;
	Fleet* m_fleet;
	GameUI* m_gameUi;
};