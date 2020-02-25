#include "Ship.h"
#include "Grid.h"
#include "AssetManager.h"

Ship::Ship(int length, bool vertical)
	: m_length(length),
	  m_gridPosition(0, -1),
	  m_vertical(vertical),
	  m_deployed(false),
	  m_deployable(false)
{
	setupSprite();
}

void Ship::rotate()
{
	m_vertical = !m_vertical;
	setRotation(90.0f - getRotation());
}

void Ship::setDeployed(bool deploy)
{
	m_deployed = deploy;
}

void Ship::setGridPosition(const sf::Vector2i& position)
{
	m_gridPosition = position;
}

void Ship::setDeployable(bool deployable)
{
	m_deployable = deployable;
}

void Ship::switchOrigin(const Grid& grid)
{
	if (m_vertical) {
		setOrigin(32.0f - grid.getCellSize() / 2, 16.0f);
	}
	else {
		setOrigin(32.0f - grid.getCellSize() / 2, 17.0f);
	}
}

int Ship::getLength() const
{
	return m_length;
}

sf::Vector2i Ship::getGridPosition() const
{
	return m_gridPosition;
}

bool Ship::isVertical() const
{
	return m_vertical;
}

bool Ship::isDeployed() const
{
	return m_deployed;
}

bool Ship::isDeployable() const
{
	return m_deployable;
}

void Ship::setupSprite()
{
	setTexture(AssetManager::getTexture("ships"));

	if (m_length == 2) {
		setTextureRect({ 0, 0, 64, 33 });
	}
	else if (m_length == 3) {
		setTextureRect({ 0, 33, 96, 33 });
	}
	else if (m_length == 4) {
		setTextureRect({ 0, 66, 128, 33 });
	}

	if (m_vertical) {
		setRotation(90.0f);
	}
}

sf::Packet& operator<<(sf::Packet& packet, const Ship& ship)
{
	return packet << (sf::Int8)ship.m_length
		<< (sf::Int32)ship.m_gridPosition.x
		<< (sf::Int32)ship.m_gridPosition.y
		<< ship.m_vertical;
}