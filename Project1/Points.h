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
		*this += dot;
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

	Point average() const
	{
		double x_avg = 0;
		double y_avg = 0;
		double z_avg = 0;

		for (const Point& pt : this->dots)
		{
			x_avg += pt.x;
			y_avg += pt.y;
			z_avg += pt.z;
		}

		size_t size = this->dots.size();

		return Point(
			x_avg / size, 
			y_avg / size, 
			z_avg / size
		);
	}

	Points& operator+=(const Point& pt)
	{
		dots.push_back(pt);
		return *this;
	}

	Points& operator+=(const Points& other)
	{
		dots.reserve(dots.size() + other.dots.size());

		dots.insert(dots.end(), other.dots.begin(), other.dots.end());

		return *this;
	}
};