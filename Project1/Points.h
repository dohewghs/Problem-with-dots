#pragma once
#include <vector>
#include "Point.h"

class Points
{
private:
	std::vector<Point> dots;

public:
	Points() :
		dots()
	{ }

	void AddPoint(Point dot)
	{
		this->dots.push_back(dot);
	}
	
	void clear()
	{
		this->dots.clear();
	}

	const std::vector<Point>& getPoints() const
	{
		return dots;
	}
};