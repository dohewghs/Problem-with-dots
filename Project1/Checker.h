#pragma once
#include "Points.h"
#include <iostream>
#include <map>
#include <numeric>

struct Zone
{
	//length, width, height
	int x;
	int y;
	int length;
	int width;

	Zone(int x, int y,int l, int w):
		x(x), y(y), length(l), width(w)
	{ }

	constexpr bool contains(const Point& point) const noexcept 
	{
		return point.x >= x && point.x < (x + length) &&
			point.y >= y && point.y < (y + width);
	}

	bool operator<(const Zone& other) const 
	{
		if (x != other.x) 
			return x < other.x;
		if (y != other.y) 
			return y < other.y;
		if (length != other.length) 
			return length < other.length;

		return width < other.width;
	}
};

class Checker
{
private:
	Zone zone;

	int zones_x;
	int zones_y;

	const double radius = 50.0;

public:
	Checker(Zone zone) :
		zone(zone),
		zones_x(10),
		zones_y(10)
	{ }

	void checkPoints(Points& points)
	{
		double size_x = zone.length / zones_x;
		double size_y = zone.width / zones_y;
		/*for (int i = 0; i < zones_x; ++i)
		{
			for (int j = 0; j < zones_y; ++j)
			{
				CheckedZone small_zone(zone.x + size_x * i, zone.y + size_y * j,size_x, size_y);

				for (Point& pt : points)
				{
					if (small_zone.contains(pt))
					{
						//std::cout << "HERE";
						pt.col.R = 255;
					}
				}
			}
		}*/

		std::map<Zone, Points> zones;

		for (Point& pt : points)
		{
			int i = static_cast<int>((pt.x - zone.x) / size_x);
			int j = static_cast<int>((pt.y - zone.y) / size_y);

			// 0<i<zones_x
			// 0<j<zones_y
			if (0 <= i && 0 <= j && i < zones_x && j < zones_y)
			{
				int x = static_cast<int>(zone.x + i * size_x);
				int y = static_cast<int>(zone.y + j * size_y);
				
				Zone zone(x, y, size_x, size_y);

				/*if (zone_indexes.find(zone) == zone_indexes.end())
					zone_indexes[zone] = Points();*/


				zones[zone].AddPoint(pt);
			}

			// маємо індекси зон, і відповідні точки які належать певній зоні
			// отже в зонах де велика кількість точок, можна шукати середнє значення між точками
		}

		double sq_radius = this->radius * this->radius;

		for (const auto& pair : zones)
		{
			if (pair.second.empty())
				continue;

			int x_avg = 0;
			int y_avg = 0;

			for (const Point& pt : pair.second)
			{
				x_avg += pt.x;
				y_avg += pt.y;
			}

			x_avg /= pair.second.size();
			y_avg /= pair.second.size();

			Point weak_avg(x_avg, y_avg, 0, color(255, 0, 0, 255));
			// weak_avg - середнє значення в зоні - квадраті

			points.AddPoint(weak_avg);

			x_avg = 0;
			y_avg = 0;

			size_t counter = 0;
			for (const Point& pt : pair.second)
			{
				double dx = pt.x - weak_avg.x;
				double dy = pt.y - weak_avg.y;

				double distance = std::sqrt(dx * dx + dy * dy);

				if (distance <= sq_radius)
				{
					x_avg += pt.x;
					y_avg += pt.y;
					++counter;
				}
			}

			if (counter > 0)
			{
				x_avg /= counter;
				y_avg /= counter;

				Point strong_avg(x_avg, y_avg, 0, color(0, 0, 255, 255));
				points.AddPoint(strong_avg);
			}

		}
	}
};

