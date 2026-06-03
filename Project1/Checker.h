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

	Points weakAverages(std::map<std::pair<int, int>, Points> zones, const AppConfig& config)
	{
		Points weakAvg;

		for (auto pair : zones)
		{
			if (pair.second.empty()) 
				continue;

			if (pair.second.size() < config.min_points_for_weak)
				continue;

			Point avg = pair.second.average();
			avg.col.R = 255;

			weakAvg.AddPoint(avg);
		}

		return weakAvg;
	}

	Points strongAverages(std::map<std::pair<int, int>, Points> zones, const AppConfig& config)
	{
		double sq_radius = config.radius * config.radius;
		double sq_merge_radius = config.merge_radius * config.merge_radius;

		std::vector<Point> temp_strong_points;

		for (const auto& pair : zones)
		{
			if (pair.second.empty())
				continue;

			if (pair.second.size() < config.min_points_for_weak)
				continue;

			Point weak_avg = pair.second.average();

			double x_strong_sum = 0.0;
			double y_strong_sum = 0.0;
			size_t counter = 0;
			for (const Point& pt : pair.second)
			{
				double dx = pt.x - weak_avg.x;
				double dy = pt.y - weak_avg.y;

				if ((dx * dx + dy * dy) <= sq_radius)
				{
					x_strong_sum += pt.x;
					y_strong_sum += pt.y;
					++counter;
				}
			}


			if (counter > config.min_points_for_strong)
			{
				double x_strong = x_strong_sum / counter;
				double y_strong = y_strong_sum / counter;

				temp_strong_points.push_back(Point(x_strong, y_strong, 0, color(0,0,255,255)));
			}
		}

		Points final_strong;
		std::vector<bool> visited(temp_strong_points.size(), false);

		for (size_t i = 0; i < temp_strong_points.size(); ++i)
		{
			if (visited[i])
				continue;

			double final_x = temp_strong_points[i].x;
			double final_y = temp_strong_points[i].y;

			size_t merged_count = 1;

			visited[i] = true;

			for (size_t j = i + 1; j < temp_strong_points.size(); ++j)
			{
				if (visited[j])
					continue;

				double dx = temp_strong_points[i].x - temp_strong_points[j].x;
				double dy = temp_strong_points[i].y - temp_strong_points[j].y;

				if ((dx * dx + dy * dy) <= sq_merge_radius)
				{
					final_x += temp_strong_points[j].x;
					final_y += temp_strong_points[j].y;
					++merged_count;

					visited[j] = true;
				}
			}

			Point absolute_center(
				final_x / merged_count,
				final_y / merged_count,
				0,
				color(0, 0, 255, 255)
			);

			final_strong.AddPoint(absolute_center);
		}

		return final_strong;
	}
};

