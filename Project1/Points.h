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

	constexpr size_t size() const noexcept
	{
		return dots.size();
	}

	constexpr bool empty() const noexcept
	{
		return dots.empty();
	}

	auto begin() const { return this->dots.begin(); }
	auto end() const { return this->dots.end(); }
	auto begin() { return this->dots.begin(); }
	auto end() { return this->dots.end(); }
};