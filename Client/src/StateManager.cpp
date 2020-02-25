#include "StateManager.h"

StateManager::StateManager() : m_currentState(nullptr)
{
}

State& StateManager::getCurrentState() const
{
	return *m_currentState;
}

void StateManager::transition(const std::string& nextState)
{
	if (m_states.find(nextState) != m_states.end()) {
		if (m_currentState != nullptr) {
			m_currentState->onExit();
		}

		m_currentState = m_states[nextState].get();
		m_currentState->onEnter();
	}
}

void StateManager::reset(const std::string& state)
{
	if (m_states.find(state) != m_states.end()) {
		m_states[state]->reset();
	}
}