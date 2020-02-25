#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Grid : public sf::Drawable, public sf::Transformable
{
	public:
	Grid(float posX, float posY, int dimX, int dimY, float cellSize);
	virtual ~Grid();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setCellColor(const sf::Vector2i& cell, const sf::Color& color);
	void resetCells();

	sf::Vector2i toCellCoords(const sf::Vector2i& windowCoords) const;
	sf::Vector2f toWindowCoords(const sf::Vector2i& cellCoords) const;
	sf::Vector2i getDimensions() const;
	float getCellSize() const;
	bool validCell(const sf::Vector2i& cell) const;
	bool contains(const sf::FloatRect& rect) const;

	private:
	void initCells();
	void initLines();

	private:
	sf::FloatRect m_gridBounds;
	sf::Vector2i m_dimension;
	float m_cellSize;
	sf::VertexArray m_cells;
	sf::VertexArray m_lines;
};