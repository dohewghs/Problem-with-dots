#pragma once
#include "Points.h"
#include <iostream>
#include <vector>
#include <numeric>
#include "AppConfig.h"
#include "Functions.h"

class Checker
{
private:
	Zone zone;

public:
	Checker(Zone zone) :
		zone(zone)
	{ }	

	std::vector<Points> zoning(const Points& source, const AppConfig& config)
	{
		double size_x = static_cast<double>(zone.length) / config.zones_x;
		double size_y = static_cast<double>(zone.width) / config.zones_y;
	
		int total_cells = config.zones_x * config.zones_y;
		std::vector<Points> zones(total_cells);

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
				size_t index = GridMath::index_1D(i, j, config.zones_x);

				zones[index].AddPoint(pt);
			}

			// маємо індекси зон, і відповідні точки які належать певній зоні
			// отже в зонах де велика кількість точок, можна шукати середнє значення між точками
		}

		return zones;
	}

	std::vector<Point> collect_local_strong(const std::vector<Points>& zones, const AppConfig& config, size_t start_index, size_t end_index)
	{
		double sq_radius = config.radius * config.radius;
		
		std::vector<Point> local_strong_points;

		for (size_t i = start_index; i < end_index; i++)
		{
			if (zones[i].empty() ||
				zones[i].size() < config.min_points_for_strong)
				continue;

			Point weak_avg = zones[i].average();

			double x_strong_sum = 0.0;
			double y_strong_sum = 0.0;
			size_t count = 0;
			for (const Point& pt : zones[i])
			{
				double dx = pt.x - weak_avg.x;
				double dy = pt.y - weak_avg.y;

				if ((dx * dx + dy * dy) <= sq_radius)
				{
					x_strong_sum += pt.x;
					y_strong_sum += pt.y;
					++count;
				}
			}

			if (count >= config.min_points_for_strong)
			{
				double x_strong = x_strong_sum / count;
				double y_strong = y_strong_sum / count;

				local_strong_points.push_back(Point(x_strong, y_strong, 0));
			}
		}

		return local_strong_points;
	}

	Points mergeCloseCenters(const std::vector<Point>& temp_strong_points, const AppConfig& config)
	{
		double sq_merge_radius = config.merge_radius * config.merge_radius;
		Points final_strong;

		std::vector<bool> visited(temp_strong_points.size(), false);

		for (size_t i = 0; i < temp_strong_points.size(); ++i)
		{
			if (visited[i])
				continue;

			double base_x = temp_strong_points[i].x;
			double base_y = temp_strong_points[i].y;

			double sum_x = base_x;
			double sum_y = base_y;
			size_t merged_count = 1;

			visited[i] = true;

			for (size_t j = i + 1; j < temp_strong_points.size(); ++j)
			{
				if (visited[j])
					continue;

				double dx = temp_strong_points[j].x - base_x;
				double dy = temp_strong_points[j].y - base_y;

				if ((dx * dx + dy * dy) <= sq_merge_radius)
				{
					sum_x += temp_strong_points[j].x;
					sum_y += temp_strong_points[j].y;
					++merged_count;

					visited[j] = true;
				}
			}

			Point absolute_center(
				sum_x / merged_count,
				sum_y / merged_count,
				0
			);

			final_strong.AddPoint(absolute_center);
		}

		return final_strong;
	}
};

