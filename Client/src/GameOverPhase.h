#pragma once

#include "State.h"

class Grid;
class Fleet;
class ServerConnection;
class GameUI;

class GameOverPhase : public State
{
	public:
	GameOverPhase(ServerConnection* server, Grid* playerGrid, Grid* enemyGrid, Fleet* fleet, GameUI* gameUi);

	virtual void init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void draw() override;

	private:
	void onGameOverMessage(bool victorious, bool fleetDestroyed);

	private:
	ServerConnection* m_server;
	Grid* m_playerGrid;
	Grid* m_enemyGrid;
	Fleet* m_fleet;
	GameUI* m_gameUi;

};