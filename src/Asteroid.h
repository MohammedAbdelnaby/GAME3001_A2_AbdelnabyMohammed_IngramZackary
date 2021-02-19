#pragma once
#ifndef __ASTEROIDS__
#define __ASTEROIDS__
#include "DisplayObject.h"
#include "TextureManager.h"
class asteroid : public DisplayObject
{
public:
	// constructors
	asteroid();

	// destructor
	~asteroid();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
	glm::vec2 getGridPosition()const;
	void setGridPosition(float col, float row);
private:
	glm::vec2 m_gridPosition;
};
#endif // !__ASTEROIDS__
