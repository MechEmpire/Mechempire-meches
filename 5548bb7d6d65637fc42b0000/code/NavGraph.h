#pragma once

#include "Vector2D.h"

//Very simple version nav graph

class NavGraph
{
public:
	
	virtual void searchPath(double sx,double sy,double ex,double ey) = 0;
	void searchPath(const Vector2D & start,const Vector2D & target)
	{
		searchPath(start.x,start.y,target.x,target.y);
	}
	virtual bool getNextPosition(Vector2D & position) = 0;
	

	//virtual void constructGraph() = 0;
};