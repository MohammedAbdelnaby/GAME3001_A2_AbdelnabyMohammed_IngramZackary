#pragma once
#ifndef _NAVIGATION_OBJECT_
#define _NAVIGATION_OBJECT_
#include "DisplayObject.h"

class NavigationObject : public DisplayObject
{
public:
	//Constructor
	NavigationObject();

	//Destructor
	~NavigationObject();

	//Inherited from DisplayObject
	virtual void draw() override = 0;
	virtual void update() override = 0;
	virtual void clean() override = 0;

	//Getters and Setters
	glm::vec2 getGridPosition() const;
	void setGridPosition(float col, float row);

private:
	glm::vec2 m_gridPosition;
};

#endif