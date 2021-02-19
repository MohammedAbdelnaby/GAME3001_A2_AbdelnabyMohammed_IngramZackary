#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Tile.h"
#include "Heuristic.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;

	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;
	float Totaldistance;
	Label* ImGuiLabel;
	Label* instructionsLabel;
	Label* instructionsLabel2;
	Label* totalPathCostLabel;


	// Pathfinding functions and objects
	void m_buildGrid();
	void m_setGridEnabled(bool state);
	std::vector<Tile*> m_pGrid;
	void m_computeTileCosts();
	void TotalCost();
	
	void m_findShortestPath();
	void m_displayPathList();
	//void m_moveShip();

	void m_debugView();

	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	//Heuristic
	Heuristic currentHeuristic;

	//open, closed, and path lists
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;
	bool test = true;
};

#endif /* defined (__PLAY_SCENE__) */