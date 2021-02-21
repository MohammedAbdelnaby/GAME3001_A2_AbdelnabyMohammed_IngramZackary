#pragma once
#ifndef __PATHWAY__
#define __PATHWAY__
#include "DisplayObject.h"
#include "TextureManager.h"
class Pathway : public DisplayObject
{
public:
	// constructors
	Pathway();

	// destructor
	~Pathway();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
	glm::vec2 getGridPosition()const;
	void setGridPosition(float col, float row);
private:
	glm::vec2 m_gridPosition;
};
#endif __PATHWAY__