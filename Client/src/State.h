#pragma once

#include <SFML/Window/Event.hpp>

namespace sf
{
	class RenderWindow;
}

class StateManager;

class State
{
	public:
	friend class StateManager;

	virtual ~State() = default;

	virtual void init();
	virtual void update(float delta);
	virtual void onEvent(const sf::Event& event);
	virtual void draw();
	virtual void onEnter();
	virtual void onExit();
	virtual void reset();
	void transition(const std::string& nextState);

	protected:
	sf::RenderWindow* m_window = nullptr;

	private:
	StateManager* m_stateManager = nullptr;

};