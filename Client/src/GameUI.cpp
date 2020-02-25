#include "GameUI.h"

#include "Network/ServerConnection.h"
#include "Game.h"

GameUI::GameUI(int enemyShips)
	: m_totalEnemyShips(enemyShips), m_enemyShipsLeft(enemyShips)
{

}

void GameUI::build(sf::RenderWindow* window, ServerConnection* server, Game* game)
{
	m_gui.setTarget(*window);

	auto playerName = tgui::Label::create();
	playerName->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	playerName->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	playerName->getRenderer()->setTextColor(tgui::Color::White);
	playerName->setPosition({ 32.0f, 16.0f });
	playerName->setSize({ 320.0f, 32.0f });
	playerName->setTextSize(16);

	auto enemyName = tgui::Label::create();
	enemyName->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	enemyName->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	enemyName->getRenderer()->setTextColor(tgui::Color::White);
	enemyName->setPosition({ 372.0f, 16.0f });
	enemyName->setSize({ 320.0f, 32.0f });
	enemyName->setTextSize(16);
	enemyName->setVisible(false);

	// DeploymentPhase UI
	auto deployInfo = tgui::Label::create(
		"Move your mouse pointer over the grid to deploy your ships.\n\n"
		"Press \"RMB\" to rotate.\n"
		"Press \"LMB\" to deploy."
	);
	deployInfo->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	deployInfo->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	deployInfo->getRenderer()->setTextColor(tgui::Color::White);
	deployInfo->setPosition({ 444.0f, 64.0f });
	deployInfo->setSize({ 160.0f, 320.0f });

	auto awaitingEnemy = tgui::Label::create("Awaiting enemy...");
	awaitingEnemy->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	awaitingEnemy->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	awaitingEnemy->getRenderer()->setTextColor(tgui::Color::White);
	awaitingEnemy->setPosition({ 444.0f, 64.0f });
	awaitingEnemy->setSize({ 160.0f, 320.0f });
	awaitingEnemy->setVisible(false);

	auto playerTurn = tgui::Label::create();
	playerTurn->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	playerTurn->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	playerTurn->getRenderer()->setTextColor(tgui::Color::White);
	playerTurn->setPosition({ 32.0f, 400.0f });
	playerTurn->setSize({ 320.0f, 32.0f });
	playerTurn->setTextSize(16);
	playerTurn->setVisible(false);

	auto enemyShipsLeft = tgui::Label::create("Enemy ships left: " + std::to_string(m_enemyShipsLeft));
	enemyShipsLeft->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	enemyShipsLeft->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	enemyShipsLeft->getRenderer()->setTextColor(tgui::Color::White);
	enemyShipsLeft->setPosition({ 374.0f, 400.0f });
	enemyShipsLeft->setSize({ 320.0f, 32.0f });
	enemyShipsLeft->setTextSize(16);
	enemyShipsLeft->setVisible(false);

	m_gui.add(playerName, "playerName");
	m_gui.add(enemyName, "enemyName");
	m_gui.add(deployInfo, "deployInfo");
	m_gui.add(awaitingEnemy, "awaitingEnemy");
	m_gui.add(playerTurn, "playerTurn");
	m_gui.add(enemyShipsLeft, "enemyShipsLeft");

	// GameOverPhase UI
	auto gameOverPanel = tgui::Panel::create({ "40%", "30%" });
	gameOverPanel->setPosition({ "30%", "35%" });
	gameOverPanel->getRenderer()->setBackgroundColor({ 60, 60, 60, 255 });
	gameOverPanel->getRenderer()->setBorders(2.0f);
	gameOverPanel->getRenderer()->setBorderColor(tgui::Color::White);

	auto vbox = tgui::VerticalLayout::create();

	auto gameResult = tgui::Label::create();
	gameResult->setTextSize(26);
	gameResult->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	gameResult->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	gameResult->getRenderer()->setTextColor(tgui::Color::White);

	auto gameResultReason = tgui::Label::create();
	gameResultReason->setTextSize(12);
	gameResultReason->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	gameResultReason->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	gameResultReason->getRenderer()->setTextColor(tgui::Color::White);

	auto playAgain = tgui::Button::create("Play again");
	playAgain->connect("pressed", [=]() {
		game->reset();
	});

	auto exit = tgui::Button::create("Exit");
	exit->connect("pressed", [=]() {
		server->disconnect();
		window->close();
	});

	auto hbox = tgui::HorizontalLayout::create();
	hbox->addSpace(0.1f);
	hbox->add(playAgain);
	hbox->addSpace(0.25f);
	hbox->add(exit);
	hbox->addSpace(0.1f);

	vbox->addSpace(0.1f);
	vbox->add(gameResult, "gameResult");
	vbox->addSpace(0.15f);
	vbox->add(gameResultReason, "gameResultReason");
	vbox->addSpace(0.35f);
	vbox->add(hbox);
	vbox->addSpace(0.2f);

	gameOverPanel->add(vbox);
	gameOverPanel->setVisible(false);

	m_gui.add(gameOverPanel, "gameOverPanel");
}

void GameUI::handleEvent(const sf::Event& event)
{
	m_gui.handleEvent(event);
}

void GameUI::draw()
{
	m_gui.draw();
}

void GameUI::setPlayerNameLabel(const std::string& playerName)
{
	m_gui.get<tgui::Label>("playerName")->setText(playerName);
}

void GameUI::setEnemyNameLabel(const std::string& enemyName)
{
	m_gui.get<tgui::Label>("enemyName")->setText(enemyName);
}

void GameUI::setTurnLabel(bool playerTurn)
{
	auto playerTurnLabel = m_gui.get<tgui::Label>("playerTurn");

	if (playerTurn) {
		playerTurnLabel->setText("Your turn");
	}
	else {
		std::string enemyName = m_gui.get<tgui::Label>("enemyName")->getText();
		playerTurnLabel->setText(enemyName + "'s turn");
	}
}

void GameUI::updateEnemyShipsLeftLabel()
{
	m_gui.get<tgui::Label>("enemyShipsLeft")->setText("Enemy ships left: " + std::to_string(--m_enemyShipsLeft));
}

void GameUI::resetEnemyShipsLeft()
{
	m_enemyShipsLeft = m_totalEnemyShips;
	m_gui.get<tgui::Label>("enemyShipsLeft")->setText("Enemy ships left: " + std::to_string(m_enemyShipsLeft));
}

void GameUI::setupGameOverPanel(bool victory, bool fleetDestroyed)
{
	auto gameResult = m_gui.get<tgui::Label>("gameResult");
	auto gameResultReason = m_gui.get<tgui::Label>("gameResultReason");

	if (victory) {
		gameResult->setText("Victory!");

		if (fleetDestroyed) {
			gameResultReason->setText("Enemy fleet was destroyed.");
		}
		else {
			gameResultReason->setText("Enemy player left the game.");
		}
	}
	else {
		gameResult->setText("Defeat!");
		gameResultReason->setText("Your fleet was destroyed.");
	}
}

void GameUI::enableEnemyNameLabel(bool enable)
{
	m_gui.get<tgui::Label>("enemyName")->setVisible(enable);
}

void GameUI::enableTurnLabel(bool enable)
{
	m_gui.get<tgui::Label>("playerTurn")->setVisible(enable);
}

void GameUI::enableGameOverPanel(bool enable)
{
	m_gui.get<tgui::Panel>("gameOverPanel")->setVisible(enable);
}

void GameUI::enableDeployInfoLabel(bool enable)
{
	m_gui.get<tgui::Label>("deployInfo")->setVisible(enable);
}

void GameUI::enableAwaitingEnemyLabel(bool enable)
{
	m_gui.get<tgui::Label>("awaitingEnemy")->setVisible(enable);
}

void GameUI::enableEnemyShipsLeftLabel(bool enable)
{
	m_gui.get<tgui::Label>("enemyShipsLeft")->setVisible(enable);
}