#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <sstream>
#include <iomanip>
// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("spacebg", 400, 300, 0, 255, true);
	drawDisplayList();
	
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	TotalCost();
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_GRAVE))
	{
		totalPathCostLabel->setEnabled(true);
		ImGuiLabel->setEnabled(false);
		instructionsLabel->setEnabled(true);
		instructionsLabel2->setEnabled(true);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

}

void PlayScene::start()
{

	// Set GUI Title
	m_guiTitle = "Play Scene";
	TextureManager::Instance()->load("../Assets/textures/spacebg.png", "spacebg");

	m_buildGrid();
	auto offSet = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	currentHeuristic = MANHATTAN;

	//Target
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(15,11)->getTransform()->position+offSet;
	m_pTarget->setGridPosition(15, 11);
	m_getTile(15, 11)->setTileStatus(GOAL);
	addChild(m_pTarget);
	
	//SpaceShip
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offSet;
	m_pSpaceShip->setGridPosition(1, 1);
	m_getTile(1, 1)->setTileStatus(START);
	addChild(m_pSpaceShip);
	m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);


	m_computeTileCosts();
	



	SoundManager::Instance().load("../Assets/audio/David Bowie.mp3", "David Bowie", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(30);
	SoundManager::Instance().playMusic("David Bowie", -1);
	const SDL_Color green = { 0, 255, 0, 255 };
	const SDL_Color white = { 255, 255, 255, 255 };
	ImGuiLabel = new Label("Press ' ` ' for debug menu", "Consolas", 40, green, glm::vec2(400.0f, 300.0f));
	ImGuiLabel->setParent(this);
	addChild(ImGuiLabel);

	instructionsLabel = new Label("START - will start the A* pathfinding algorithm ", "Consolas", 20, white, glm::vec2(400.0f, 550.0f));
	instructionsLabel->setParent(this);
	addChild(instructionsLabel);
	instructionsLabel->setEnabled(false);

	instructionsLabel2 = new Label("RESTART - will restart the ship at (1,1) and the target at (15,11)", "Consolas", 20, white, glm::vec2(400.0f, 575.0f));
	instructionsLabel2->setParent(this);
	addChild(instructionsLabel2);
	instructionsLabel2->setEnabled(false);

	totalPathCostLabel = new Label("--", "Consolas", 20, white, glm::vec2(400.0f, 525.0f));
	totalPathCostLabel->setParent(this);
	addChild(totalPathCostLabel);
	totalPathCostLabel->setEnabled(false);
	TotalCost();

}

void PlayScene::GUI_Function() 
{
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - A2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool isGridEnabled = false;
	if(ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		// toggle grid on/off
		m_setGridEnabled(isGridEnabled);

	}


	ImGui::Separator();
	auto offSet = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	//auto radio = static_cast<int>(currentHeuristic);
	//ImGui::LabelText("", "Heuristic Type");
	//ImGui::RadioButton("Manhattan", &radio, static_cast<int>(MANHATTAN));
	//ImGui::SameLine();
	//ImGui::RadioButton("Euclidean", &radio, static_cast<int>(EUCLIDEAN));
	//if (currentHeuristic != Heuristic(radio))
	//{
	//	currentHeuristic = Heuristic(radio);
	//	m_computeTileCosts();
	//}

	ImGui::Separator();

	//static int startPosition[] = { m_pSpaceShip->getGridPosition().x, m_pSpaceShip->getGridPosition().y };
	//if (ImGui::SliderInt2("Start position", startPosition, 0, Config::COL_NUM - 1))
	//{
	//	if (startPosition[1] > Config::ROW_NUM - 1)
	//	{
	//		startPosition[1] = Config::ROW_NUM - 1;
	//	}
	//	SDL_RenderClear(Renderer::Instance()->getRenderer());
	//	m_pSpaceShip->getTransform()->position = m_getTile(startPosition[0], startPosition[1])->getTransform()->position + offSet;
	//	m_pSpaceShip->setGridPosition(startPosition[0], startPosition[1]);
	//
	//	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	//	SDL_RenderPresent(Renderer::Instance()->getRenderer());
	//}

	static int targetPosition[] = { m_pTarget->getGridPosition().x, m_pTarget->getGridPosition().y };
	if (ImGui::SliderInt2("Target position", targetPosition, 0, Config::COL_NUM - 1))
	{
		if (targetPosition[1] > Config::ROW_NUM -1)
		{
			targetPosition[1] = Config::ROW_NUM - 1;
		}
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
		m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position +offSet;
		m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	
	
	ImGui::Separator();

	//Start position/Spaceship position
	static int spaceShipPosition[] = { m_pSpaceShip->getGridPosition().x, m_pSpaceShip->getGridPosition().y };
	if (ImGui::SliderInt2("Spaceship position", spaceShipPosition, 0, Config::COL_NUM - 1))
	{
		if (spaceShipPosition[1] > Config::ROW_NUM - 1)
		{
			spaceShipPosition[1] = Config::ROW_NUM - 1;
		}
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);
		m_pSpaceShip->getTransform()->position = m_getTile(spaceShipPosition[0], spaceShipPosition[1])->getTransform()->position + offSet;
		m_pSpaceShip->setGridPosition(spaceShipPosition[0], spaceShipPosition[1]);
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);
		//m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	ImGui::Separator();
	
	if(ImGui::Button("Start"))
	{
		SoundManager::Instance().load("../Assets/audio/Menu Selection Click.wav", "Menu Selection Click", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(50);
		SoundManager::Instance().playSound("Menu Selection Click", 0, 0);
		


			m_findShortestPath();

	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offSet;
		m_pTarget->setGridPosition(15, 11);
		
		
		m_pSpaceShip->getTransform()->position = m_getTile(1,1)->getTransform()->position + offSet;
		m_pSpaceShip->setGridPosition(1, 1);
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());

		for (auto node : m_pGrid)
		{
			node->setTileStatus(UNVISITED);
		}

		

		SoundManager::Instance().load("../Assets/audio/teleport.wav", "teleport", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(25);
		SoundManager::Instance().playSound("teleport", 0, 0);
	}

	ImGui::Separator();

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;
	
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabel();
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}
	// create refreences to its negithebtour 
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row -1));
			}


			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);

			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));
			}


			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));
			}


			if (col==0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}
		}
	}
	std::cout << m_pGrid.size() << std::endl;
}

void PlayScene::m_setGridEnabled(bool state) 
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}

	if(state == false)
	{
		SDL_RenderClear(Renderer::Instance()->getRenderer());
	}
}

void PlayScene::m_computeTileCosts()
{
	float distance;

	for (auto tile : m_pGrid)
	{
		//switch (currentHeuristic)
		//{
		//case MANHATTAN:
		//	//Manhattan distance
		//	distance = abs(tile->getGridPosition().x - m_pTarget->getGridPosition().x) + abs(tile->getGridPosition().y - m_pTarget->getGridPosition().y);
		//	break;
		//case EUCLIDEAN:
		//	//Euclidean distance
		//	distance = Util::distance(m_pTarget->getGridPosition(), tile->getGridPosition());
		//	break;
		//}

		distance = abs(tile->getGridPosition().x - m_pTarget->getGridPosition().x) + abs(tile->getGridPosition().y - m_pTarget->getGridPosition().y);

		tile->setTileCost(distance);
	}

}

void PlayScene::TotalCost()
{
	auto m_cost = abs(m_pSpaceShip->getGridPosition().x - m_pTarget->getGridPosition().x) + abs(m_pSpaceShip->getGridPosition().y - m_pTarget->getGridPosition().y);
	std::stringstream stream;

	stream << std::fixed << std::setprecision(1) <<"Total Cost: "<< m_cost;
	const std::string cost_string = stream.str();
	totalPathCostLabel->setText(cost_string);
}

void PlayScene::m_findShortestPath()
{

	m_pClosedList.clear();
	m_pClosedList.shrink_to_fit();
	m_pOpenList.clear();
	m_pOpenList.shrink_to_fit();
	m_pPathList.clear();
	m_pPathList.shrink_to_fit();
	//Add start position to the open list
	auto startTile = m_getTile(m_pSpaceShip->getGridPosition());
	startTile->setTileStatus(OPEN);
	m_pOpenList.push_back(startTile);

	bool goalFound = false;

	//Loop until the open list is empty or the goal is found
	while (!m_pOpenList.empty() && !goalFound)
	{
		auto min = INFINITY;
		Tile* minTile;
		int minTileIndex = 0;
		int count = 0;

		std::vector<Tile*> neighbourList;
		for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; index++)
		{
			neighbourList.push_back(m_pOpenList[0]->getNeighbourTile(NeighbourTile(index)));
		}

		for (auto neighbour : neighbourList)
		{
			std::cout << "TEST " << std::endl;
			if (neighbour->getTileStatus() != GOAL)
			{
				if (neighbour->getTileCost() < min)
				{
					min = neighbour->getTileCost();
					minTile = neighbour;
					minTileIndex = count;
				}
				count++;
			}
			else
			{
				minTile = neighbour;
				m_pPathList.push_back(minTile);
				goalFound = true;
				break;
			}
		}

		//remove the reference of the current tile in the open list
		m_pPathList.push_back(m_pOpenList[0]);
		m_pOpenList.pop_back(); //empties the list

		//add minTile to the open list
		m_pOpenList.push_back(minTile);
		minTile->setTileStatus(OPEN);
		neighbourList.erase(neighbourList.begin() + minTileIndex);

		//push all remaing neighbours onto the closed list
		for (auto neighbour : neighbourList)
		{
			if (neighbour->getTileStatus() == UNVISITED)
			{
				neighbour->setTileStatus(CLOSED);
				m_pClosedList.push_back(neighbour);
			}
		}
	}
	m_displayPathList();
}

void PlayScene::m_displayPathList()
{
	for (auto node : m_pPathList)
	{
		std::cout << "(" << node->getGridPosition().x << "," << node->getGridPosition().y << ")" << std::endl;
	}
	std::cout << "Path Length: " << m_pPathList.size() << std::endl;

}

Tile* PlayScene::m_getTile(int col, int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(const glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}
