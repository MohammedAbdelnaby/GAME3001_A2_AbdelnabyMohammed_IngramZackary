#include "Asteroid.h"

asteroid::asteroid()
{
	TextureManager::Instance()->load("../Assets/textures/asteroid.png", "asteroid");

	auto size = TextureManager::Instance()->getTextureSize("asteroid");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(300.0f, 300.0f);
}


asteroid::~asteroid()
= default;

void asteroid::draw()
{
	TextureManager::Instance()->draw("asteroid",getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void asteroid::update()
{
}

void asteroid::clean()
{
}

glm::vec2 asteroid::getGridPosition() const
{
	return m_gridPosition;
}

void asteroid::setGridPosition(const float col, const float row)
{
	m_gridPosition = glm::vec2(col, row);
}
