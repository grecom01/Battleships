#include "State.h"
#include "StateManager.h"

void State::init()
{
}

void State::update(float delta)
{
}

void State::onEvent(const sf::Event& event)
{
}

void State::draw()
{
}

void State::onEnter()
{
}

void State::onExit()
{
}

void State::reset()
{
}

void State::transition(const std::string& nextState)
{
	m_stateManager->transition(nextState);
}
