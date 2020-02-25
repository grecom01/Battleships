#include "Application.h"
#include "AssetManager.h"
#include "Login.h"
#include "Game.h"

Application::Application(const std::string& title, int windowWidth, int windowHeight)
	: m_window(
		sf::VideoMode(windowWidth, windowHeight),
		title,
		sf::Style::Close | sf::Style::Titlebar
	  ),
	  m_messageQueue(),
	  m_server(&m_messageQueue)
{
}

void Application::loadAssets()
{ 
	AssetManager::addTexture("res/BattleshipsTextureMap.png", "ships");
}

void Application::initStates()
{
	m_stateManager.addState<Login>("login", &m_window, &m_server);
	m_stateManager.addState<Game>("game", &m_window, &m_server);
}

void Application::run()
{
	loadAssets();
	initStates();

	sf::Event event;
	sf::Clock clock;

	while (m_window.isOpen()) {
		m_messageQueue.processMessages();

		State& current = m_stateManager.getCurrentState();

		while (m_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_server.disconnect();
				m_window.close();
			}

			current.onEvent(event);
		}

		current.update(clock.restart().asSeconds());

		m_window.clear({ 60, 60, 60, 255 });
		current.draw();
		m_window.display();
	}
}