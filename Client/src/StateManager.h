#pragma once

#include "State.h"

#include <unordered_map>
#include <type_traits>

class StateManager
{
	public:
	StateManager();

	State& getCurrentState() const;
	void transition(const std::string& nextState);
	void reset(const std::string& state);

	template <typename T, typename ... Args>
	void addState(const std::string& stateName, sf::RenderWindow* window, Args&&... args)
	{
		static_assert(std::is_base_of<State, T>::value, "Type must inherit from State");

		if (m_states.try_emplace(stateName, std::make_unique<T>(std::forward<Args>(args)...)).second) {
			m_states[stateName]->m_window = window;
			m_states[stateName]->m_stateManager = this;
			m_states[stateName]->init();
		}

		if (m_states.find(stateName) != m_states.end() && m_states.size() == 1) {
			m_currentState = m_states[stateName].get();
		}
	}

	private:
	State* m_currentState = nullptr;
	std::unordered_map<std::string, std::unique_ptr<State>> m_states;
};