#include "Pathway.h"

Pathway::Pathway()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png", "Circle");

	auto size = TextureManager::Instance()->getTextureSize("Circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(300.0f, 300.0f);
}

Pathway::~Pathway()
= default;
void Pathway::draw()
{
	TextureManager::Instance()->draw("Circle", getTransform()->position.x, getTransform()->position.y, 0, 255, true);

}

void Pathway::update()
{
}

void Pathway::clean()
{
}

glm::vec2 Pathway::getGridPosition() const
{
	return m_gridPosition;
}

void Pathway::setGridPosition(float col, float row)
{
	m_gridPosition = glm::vec2(col, row);
}
