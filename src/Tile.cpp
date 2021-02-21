#include "Tile.h"
#include <sstream>
#include "Config.h"
#include "TextureManager.h"
#include "Util.h"
#include <iomanip>

Tile::Tile(): m_cost(0.0f)
{
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
}

Tile::~Tile()
= default;

void Tile::draw()
{
	Util::DrawRect(getTransform()->position, getWidth(), getHeight());
}

void Tile::update()
{
}

void Tile::clean()
{
}

Tile* Tile::getNeighbourTile(const NeighbourTile position)
{
	return m_neighbours[position];
}

void Tile::setNeighbourTile(const NeighbourTile position, Tile* tile)
{
	m_neighbours[position] = tile;
}

float Tile::getTileCost() const
{
	return m_cost;
}

void Tile::setTileCost(const float cost)
{
	m_cost = cost;

	std::stringstream stream;

	stream << std::fixed << std::setprecision(1) << cost;
	const std::string cost_string = stream.str();
	m_costLabel->setText(cost_string);
}

void Tile::addLabel()
{
	const SDL_Color white = {0, 255, 255 ,255};
	auto offSet = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_costLabel = new Label("99.9", "Consolas", 12, white);
	m_costLabel->getTransform()->position = getTransform()->position + offSet + glm::vec2(0.0f, -6.0f);
	getParent()->addChild(m_costLabel);
	m_costLabel->setEnabled(false);


	m_statusLabel = new Label("--", "Consolas", 12, white);
	m_statusLabel->getTransform()->position = getTransform()->position + offSet + glm::vec2(0.0f, +6.0f);
	getParent()->addChild(m_statusLabel);
	m_statusLabel->setEnabled(false);
}

void Tile::setLabelsEnabled(const bool state)
{
	m_costLabel->setEnabled(state);
	m_statusLabel->setEnabled(state);
}

TileStatus Tile::getTileStatus() const
{
	return m_status;
}

void Tile::setTileStatus(const TileStatus status)
{
	m_status = status;

	switch (status)
	{
	case UNVISITED:
		m_statusLabel->setText("--");
		break;
	case OPEN:
		m_statusLabel->setText("O");
		break;
	case CLOSED:
		m_statusLabel->setText("C");
		break;
	case IMPASSIBLE:
		m_statusLabel->setText("I");
		break;
	case GOAL:
		m_statusLabel->setText("G");
		break;
	case START:
		m_statusLabel->setText("S");
		break;

	}
}

