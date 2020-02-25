#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/Packet.hpp>

class Grid;

class Ship : public sf::Sprite
{
	public:
	Ship(int length, bool vertical = true);

	void rotate();
	void setDeployed(bool deploy);
	void setGridPosition(const sf::Vector2i& position);
	void setDeployable(bool deployable);
	void switchOrigin(const Grid& grid);

	int getLength() const;
	sf::Vector2i getGridPosition() const;
	bool isVertical() const;
	bool isDeployed() const;
	bool isDeployable() const;
	friend sf::Packet& operator<<(sf::Packet& packet, const Ship& ship);

	private:
	void setupSprite();

	private:
	int m_length;
	sf::Vector2i m_gridPosition;
	bool m_vertical;
	bool m_deployed;
	bool m_deployable;
};