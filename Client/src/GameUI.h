#pragma once

#include <TGUI/TGUI.hpp>

class ServerConnection;
class Game;

class GameUI
{
	public:
	GameUI(int enemyShips);

	void build(sf::RenderWindow* window, ServerConnection* server, Game* game);
	void handleEvent(const sf::Event& event);
	void draw();

	void setPlayerNameLabel(const std::string& playerName);
	void setEnemyNameLabel(const std::string& enemyName);
	void setTurnLabel(bool playerMove);
	void updateEnemyShipsLeftLabel();
	void resetEnemyShipsLeft();
	void setupGameOverPanel(bool victory, bool fleetDestroyed);
	void enableEnemyNameLabel(bool enable);
	void enableTurnLabel(bool enable);
	void enableGameOverPanel(bool enable);
	void enableDeployInfoLabel(bool enable);
	void enableAwaitingEnemyLabel(bool enable);
	void enableEnemyShipsLeftLabel(bool enable);

	private:
	const int m_totalEnemyShips;
	int m_enemyShipsLeft;
	tgui::Gui m_gui;
};