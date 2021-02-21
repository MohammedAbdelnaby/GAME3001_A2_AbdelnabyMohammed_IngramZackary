#pragma once
#ifndef __TILE__
#define __TILE__
#include "NavigationObject.h"
#include "NeighbourTile.h"
#include "Label.h"
#include "TileStatus.h"

class Tile : public NavigationObject
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

	TileStatus getTileStatus() const;
	void setTileStatus(const TileStatus status);

private:
	TileStatus m_status;
	
	Label* m_costLabel;
	Label* m_statusLabel;
	float m_cost;
	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];
	//glm::vec2 m_gridPosition;
};

#endif /* defined (__TILE__) */