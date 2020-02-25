#pragma once

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Packet;
}

class Ship
{
	public:
	Ship();

	bool takeHit(const sf::Vector2i& attackCoords);
	void setPosition(const sf::Vector2i& position);
	friend sf::Packet& operator>>(sf::Packet&, Ship& ship);
	void printStatus();

	bool hasSunk() const;

	private:
	sf::Int8 m_length;
	sf::Int8 m_hitPoints;
	sf::Vector2i m_position;
	bool m_vertical;
};
