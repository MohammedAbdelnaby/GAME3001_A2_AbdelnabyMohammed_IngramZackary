#pragma once
#ifndef __TILE__
#define __TILE__
#include "DisplayObject.h"
#include "NeighbourTile.h"
#include "Label.h"
class Tile : public DisplayObject
{
public:
	// Constructor
	Tile();
	
	// Destructor
	~Tile();
	
	// Life-Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;
	Tile* getNeighbourTile(NeighbourTile position);
	void setNeighbourTile(NeighbourTile position, Tile* tile);
	float getTileCost() const;
	void setTileCost(float cost);
	void addLabel();
	void setLabelsEnabled(bool state);
	glm::vec2 getGridPosition()const;
	void setGridPosition(float col, float row);
private:
	Label* m_costLabel;
	Label* m_statusLabel;
	float m_cost;
	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];
	glm::vec2 m_gridPosition;
};

#endif /* defined (__TILE__) */