#include "Grid.h"
#include "AssetManager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

Grid::Grid(float posX, float posY, int dimX, int dimY, float cellSize)
    : m_gridBounds(posX - 1.0f, posY - 1.0f, (float)dimX * cellSize + 2.0f, (float)dimY * cellSize + 2.0f),
      m_dimension(dimX, dimY),
      m_cellSize(cellSize)
{
    setPosition(posX, posY);
    initCells();
    initLines();
}

Grid::~Grid()
{
}

void Grid::initCells()
{
    m_cells.setPrimitiveType(sf::Quads);
    m_cells.resize(4 * m_dimension.x * m_dimension.y);

    for (int i = 0; i < m_dimension.x; ++i) {
        for (int j = 0; j < m_dimension.y; ++j) {
            int vertexIndex = (j + i * m_dimension.x) * 4;
            sf::Vertex* quad = &m_cells[vertexIndex];

            quad[0].position = sf::Vector2f(i * m_cellSize, j * m_cellSize);
            quad[1].position = sf::Vector2f((i + 1) * m_cellSize, j * m_cellSize);
            quad[2].position = sf::Vector2f((i + 1) * m_cellSize, (j + 1) * m_cellSize);
            quad[3].position = sf::Vector2f(i * m_cellSize, (j + 1) * m_cellSize);

            quad[0].color = sf::Color::Transparent;
            quad[1].color = sf::Color::Transparent;
            quad[2].color = sf::Color::Transparent;
            quad[3].color = sf::Color::Transparent;
        }
    }
}

void Grid::initLines()
{
    int vertexHLines = 2 * (m_dimension.y + 1);
    int vertexVLines = 2 * (m_dimension.x + 1);

    m_lines.setPrimitiveType(sf::Lines);
    m_lines.resize(vertexHLines + vertexVLines);

    // Horizontal lines
    for (int i = 0; i < vertexHLines; i += 2) {
        m_lines[i].position = { 0.0f, (i / 2) * m_cellSize };
        m_lines[i + 1].position = { m_dimension.x * m_cellSize, (i / 2) * m_cellSize };

        m_lines[i].color = sf::Color::Black;
        m_lines[i + 1].color = sf::Color::Black;
    }

    // Vertical line
    for (int i = 0; i < vertexVLines; i += 2) {
        m_lines[vertexHLines + i].position = { (i / 2) * m_cellSize, 0.0f };
        m_lines[vertexHLines + i + 1].position = { (i / 2) * m_cellSize, m_dimension.y * m_cellSize };

        m_lines[vertexHLines + i].color = sf::Color::Black;
        m_lines[vertexHLines + i + 1].color = sf::Color::Black;
    }
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform = getTransform();
    target.draw(m_cells, states);
    target.draw(m_lines, states);
}

sf::Vector2i Grid::toCellCoords(const sf::Vector2i& windowCoords) const
{
    sf::Vector2i position = static_cast<sf::Vector2i>(getPosition());

    return { 
        (windowCoords.x - position.x) / (int)m_cellSize,
        (windowCoords.y - position.y) / (int)m_cellSize
    };
}

sf::Vector2f Grid::toWindowCoords(const sf::Vector2i& cellCoords) const
{
    sf::Vector2f position = getPosition();

    return {
        m_cellSize * cellCoords.x + position.x,
        m_cellSize * cellCoords.y + position.y
    };
}

sf::Vector2i Grid::getDimensions() const
{
    return m_dimension;
}

float Grid::getCellSize() const
{
    return m_cellSize;
}

void Grid::setCellColor(const sf::Vector2i& cell, const sf::Color& color)
{
    if (validCell(cell)) {
        int index = (cell.y + cell.x * m_dimension.x) * 4;
        sf::Vertex* quad = &m_cells[index];

        quad[0].color = color;
        quad[1].color = color;
        quad[2].color = color;
        quad[3].color = color;
    }
}

void Grid::resetCells()
{
    for (int i = 0; i < m_dimension.x; ++i) {
        for (int j = 0; j < m_dimension.y; ++j) {
            int vertexIndex = (j + i * m_dimension.x) * 4;
            sf::Vertex* quad = &m_cells[vertexIndex];

            quad[0].color = sf::Color::Transparent;
            quad[1].color = sf::Color::Transparent;
            quad[2].color = sf::Color::Transparent;
            quad[3].color = sf::Color::Transparent;
        }
    }
}

bool Grid::validCell(const sf::Vector2i& cell) const
{
    return 0 <= cell.x && cell.x < m_dimension.x && 0 <= cell.y && cell.y < m_dimension.y;
}

bool Grid::contains(const sf::FloatRect& rect) const
{
    return m_gridBounds.left < rect.left
        && rect.left + rect.width < m_gridBounds.left + m_gridBounds.width
        && m_gridBounds.top < rect.top
        && rect.top + rect.height < m_gridBounds.top + m_gridBounds.height;
}