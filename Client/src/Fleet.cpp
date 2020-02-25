#include "Fleet.h"
#include "Grid.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Network/Packet.hpp>

Fleet::Fleet(int totalSize, const Grid& grid) : m_totalSize(totalSize)
{
	sf::Vector2i dim = grid.getDimensions();
	m_shipRadar = std::vector<Ship*>(dim.x * dim.y, nullptr);
	m_ships.reserve(m_totalSize);
	m_NextUndeployed = m_ships.begin();
}

void Fleet::createShips(int length, int count)
{
	if (m_ships.size() + count <= m_totalSize && count > 0) {
		for (int i = 0; i < count; ++i) {
			m_ships.emplace_back(length);
		}
	}

	if (m_ships.size() == m_totalSize) {
		m_NextUndeployed = m_ships.begin();
	}
}

void Fleet::deployShip(const Grid& grid, Ship& ship)
{
	sf::Vector2i offset = ship.isVertical() ? sf::Vector2i(0, 1) : sf::Vector2i(1, 0);
	sf::Vector2i gridPos = ship.getGridPosition();

	for (int i = 0; i < ship.getLength(); ++i) {
		sf::Vector2i cell = gridPos + i * offset;

		m_shipRadar[grid.getDimensions().x * cell.y + cell.x] = &ship;
	}

	ship.setColor(sf::Color::White);
	ship.setDeployed(true);
	ship.setDeployable(false);
}

void Fleet::resetShips()
{
	for (Ship& ship : m_ships) {
		ship.setDeployed(false);
	}

	for (size_t i = 0; i < m_shipRadar.size(); ++i) {
		m_shipRadar[i] = nullptr;
	}

	m_NextUndeployed = m_ships.begin();
}

void Fleet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ship& ship : m_ships) {
		if (ship.isDeployed()) {
			target.draw(ship);
		}
	}
}

int Fleet::getTotalSize() const
{
	return m_totalSize;
}

bool Fleet::isFullyDeployed() const
{
	for (const Ship& ship : m_ships) {
		if (!ship.isDeployed()) {
			return false;
		}
	}

	return m_ships.size() == m_totalSize;
}

bool Fleet::shipOnGridCell(const Grid& grid, const sf::Vector2i& cell) const
{
	return m_shipRadar[grid.getDimensions().x * cell.y + cell.x] != nullptr;
}

Ship* Fleet::getUndeployedShip()
{
	if (m_NextUndeployed == m_ships.end()) {
		return nullptr;
	}

	if (m_NextUndeployed->isDeployed()) {
		++m_NextUndeployed;

		if (m_NextUndeployed == m_ships.end()) {
			return nullptr;
		}

	}

	return &*m_NextUndeployed;
}

sf::Packet& operator<<(sf::Packet& packet, const Fleet& fleet)
{
	packet << (sf::Uint8)fleet.m_ships.size();

	for (size_t i = 0; i < fleet.m_ships.size(); ++i) {
		packet << fleet.m_ships[i];
	}

	return packet;
}