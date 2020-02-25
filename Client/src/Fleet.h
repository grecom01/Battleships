#pragma once

#include "Ship.h"

#include <vector>

class Fleet : public sf::Drawable
{
	public:
	Fleet(int totalSize, const Grid& grid);

	void createShips(int length, int count);
	void deployShip(const Grid& grid, Ship& ship);
	void resetShips();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	

	int getTotalSize() const;
	bool isFullyDeployed() const;
	bool shipOnGridCell(const Grid& grid, const sf::Vector2i& cell) const;
	Ship* getUndeployedShip();
	friend sf::Packet& operator<<(sf::Packet& packet, const Fleet& fleet);

	private:
	int m_totalSize;
	std::vector<Ship*> m_shipRadar;
	std::vector<Ship> m_ships;
	std::vector<Ship>::iterator m_NextUndeployed;
};