#include "Ship.h"


#include <SFML/Network/Packet.hpp>
#include <iostream>

Ship::Ship()
	: m_length(0),
	  m_hitPoints(0),
	  m_position(0, 0),
	  m_vertical(false)
{
}

bool Ship::takeHit(const sf::Vector2i& attackCoords)
{
	sf::Vector2i offset = m_vertical ? sf::Vector2i(0, 1) : sf::Vector2i(1, 0);

	for (sf::Int32 i = 0; i < m_length; ++i) {
		if (m_position + i * offset == attackCoords) {
			--m_hitPoints;			
			return true;
		}
	}

	return false;
}

void Ship::setPosition(const sf::Vector2i& position)
{
	m_position = position;
}

bool Ship::hasSunk() const
{
	return m_hitPoints == 0;
}

sf::Packet& operator>>(sf::Packet& packet, Ship& ship)
{
	packet >> ship.m_length
	       >> ship.m_position.x
		   >> ship.m_position.y
		   >> ship.m_vertical;

	ship.m_hitPoints = ship.m_length;

	return packet;
}

void Ship::printStatus()
{
	std::cout << "\n-----------"
		<< "\nPos: [" << m_position.x << "," << m_position.y << "]"
		<< "\nLen: " << (int)m_length
		<< "\nVert: " << m_vertical
		<< "\nHP: " << (int)m_hitPoints << std::endl;
}