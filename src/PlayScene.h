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
	void TotalCost(const float cost);
	


	Tile* m_getTile(int col, int row);
	
};

#endif /* defined (__PLAY_SCENE__) */