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
#include "Asteroid.h"
#include "Pathway.h"

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
	Label* instructionsLabel3;
	Label* instructionsLabel4;
	Label* instructionsLabel5;
	Label* totalPathCostLabel;


	// Pathfinding functions and objects
	void m_buildGrid();
	void m_setGridEnabled(bool state);
	bool m_getGridEnabled();
	std::vector<Tile*> m_pGrid;
	void m_computeTileCosts();
	void TotalCost();
	void m_findShortestPath();
	void m_displayPathList();
	void m_setInstructionsEnabled(bool enabled);
	bool m_getInstructionsEnabled();
	void m_createPathway();

	bool m_GridEnabled;
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);
	asteroid* m_pAsteroids[8];
	//Heuristic
	Heuristic currentHeuristic;
	int cooldown = 20;
	//open, closed, and path lists
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;
	bool test = true;
	bool InstructionsEnabled = true;
	void m_moveShip();
	int moveCounter = 0;
	bool m_shipIsMoving = false;
	std::vector<Pathway*> m_pPathway;

};

#endif /* defined (__PLAY_SCENE__) */