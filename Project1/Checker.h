#pragma once
#include "Points.h"
#include <iostream>
#include <map>
#include <numeric>
#include "AppConfig.h"

struct Zone
{
	//length, width, height
	double x;
	double y;
	double length;
	double width;

	Zone(double x, double y, double l, double w):
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

public:
	Checker(Zone zone) :
		zone(zone)
	{ }

	//void checkPoints(const Points& points)
	//{
	//	

	//	double sq_radius = this->radius * this->radius;

	//	for (const auto& pair : zones)
	//	{
	//		if (pair.second.empty())
	//			continue;

	//		Point weak_avg = pair.second.average();
	//		weak_avg.col.R = 255;
	//		// weak_avg - середнє значення в зоні - квадраті

	//		points.AddPoint(weak_avg);

	//		double x_avg = 0;
	//		double y_avg = 0;

	//		size_t counter = 0;
	//		for (const Point& pt : pair.second)
	//		{
	//			double dx = pt.x - weak_avg.x;
	//			double dy = pt.y - weak_avg.y;

	//			double distance = dx * dx + dy * dy;

	//			if (distance <= sq_radius)
	//			{
	//				x_avg += pt.x;
	//				y_avg += pt.y;
	//				++counter;
	//			}
	//		}

	//		if (counter > 3)
	//		{
	//			x_avg /= static_cast<double>(counter);
	//			y_avg /= static_cast<double>(counter);

	//			Point strong_avg(x_avg, y_avg, 0, color(0, 0, 255, 255));
	//			points.AddPoint(strong_avg);
	//		}

	//	}
	//}

	std::map<std::pair<int, int>, Points> zoning(const Points& source, const AppConfig& config)
	{
		double size_x = static_cast<double>(zone.length) / config.zones_x;
		double size_y = static_cast<double>(zone.width) / config.zones_y;
	
		std::map<std::pair<int, int>, Points> zones;

		/*for (int i = 0; i < zones_x; ++i)
		{
			for (int j = 0; j < zones_y; ++j)
			{
				CheckedZone small_zone(zone.x + size_x * i, zone.y + size_y * j,size_x, size_y);

				for (Point& pt : points)
				{
					if (small_zone.contains(pt))
					{
					}
				}
			}
		}*/

		for (const Point& pt : source)
		{
			int i = static_cast<int>((pt.x - zone.x) / size_x);
			int j = static_cast<int>((pt.y - zone.y) / size_y);

			// 0<i<zones_x
			// 0<j<zones_y
			if (0 <= i && 0 <= j && i < config.zones_x && j < config.zones_y)
			{
				int x = static_cast<int>(zone.x + i * size_x);
				int y = static_cast<int>(zone.y + j * size_y);

				zones[{i, j}].AddPoint(pt);
			}

			// маємо індекси зон, і відповідні точки які належать певній зоні
			// отже в зонах де велика кількість точок, можна шукати середнє значення між точками
		}

		return zones;
	}

	Points weakAverages(const Points& points, const AppConfig& config)
	{
		std::map<std::pair<int, int>, Points> zones = this->zoning(points, config);

		Points weakAvg;
		for (auto pair : zones)
		{
			if (pair.second.empty()) 
				continue;

			Point avg = pair.second.average();
			avg.col.R = 255;

			weakAvg.AddPoint(avg);
		}

		return weakAvg;
	}
};

