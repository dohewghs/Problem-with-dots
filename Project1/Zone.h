#pragma once
#include "Point.h"

struct Zone
{
	double x;
	double y;
	double length;
	double width;

	Zone(double x, double y, double l, double w) :
		x(x), y(y), length(l), width(w)
	{
	}

	constexpr bool contains(const Point& point) const noexcept
	{
		return point.x >= x && point.x < (x + length) &&
			point.y >= y && point.y < (y + width);
	}
};