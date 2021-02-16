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
	Totaldistance = Util::distance(m_pTarget->getGridPosition(), m_pSpaceShip->getGridPosition());
	TotalCost(Totaldistance);
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
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(15,11)->getTransform()->position+offSet;
	m_pTarget->setGridPosition(15, 11);
	addChild(m_pTarget);


	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offSet;
	m_pSpaceShip->setGridPosition(1, 1);
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

	Totaldistance = Util::distance(m_pTarget->getGridPosition(), m_pSpaceShip->getGridPosition());
	totalPathCostLabel = new Label("--", "Consolas", 20, white, glm::vec2(400.0f, 525.0f));
	totalPathCostLabel->setParent(this);
	addChild(totalPathCostLabel);
	totalPathCostLabel->setEnabled(false);
	TotalCost(Totaldistance);

}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 3", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool isGridEnabled = false;
	if(ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		// toggle grid on/off
		m_setGridEnabled(isGridEnabled);

	}

	ImGui::Separator();
	auto offSet = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	static int targetPosition[] = { m_pTarget->getGridPosition().x, m_pTarget->getGridPosition().y };
	if (ImGui::SliderInt2("Target position", targetPosition, 0, Config::COL_NUM - 1))
	{
		if (targetPosition[1] > Config::ROW_NUM -1)
		{
			targetPosition[1] = Config::ROW_NUM - 1;
		}
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position +offSet;
		m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	
	
	ImGui::Separator();

	static int spaceShipPosition[] = { m_pSpaceShip->getGridPosition().x, m_pSpaceShip->getGridPosition().y };
	if (ImGui::SliderInt2("Spaceship position", spaceShipPosition, 0, Config::COL_NUM - 1))
	{
		if (spaceShipPosition[1] > Config::ROW_NUM - 1)
		{
			spaceShipPosition[1] = Config::ROW_NUM - 1;
		}
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pSpaceShip->getTransform()->position = m_getTile(spaceShipPosition[0], spaceShipPosition[1])->getTransform()->position + offSet;
		m_pSpaceShip->setGridPosition(spaceShipPosition[0], spaceShipPosition[1]);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	ImGui::Separator();
	
	if(ImGui::Button("Start"))
	{
		SoundManager::Instance().load("../Assets/audio/Menu Selection Click.wav", "Menu Selection Click", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(50);
		SoundManager::Instance().playSound("Menu Selection Click", 0, 0);
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
	for (auto tile : m_pGrid)
	{
		auto distance = Util::distance(m_pTarget->getGridPosition(), tile->getGridPosition());
		tile->setTileCost(distance);
	}

}

void PlayScene::TotalCost(const float cost)
{

	std::stringstream stream;

	stream << std::fixed << std::setprecision(1) <<"Total Cost: "<< cost;
	const std::string cost_string = stream.str();
	totalPathCostLabel->setText(cost_string);
}

Tile* PlayScene::m_getTile(int col, int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}
