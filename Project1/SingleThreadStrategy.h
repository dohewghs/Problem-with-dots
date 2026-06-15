#pragma once
#include "IClusteringStrategy.h"
#include "Checker.h"

class SingleThreadStrategy : public IClusteringStrategy
{
private:
	Checker checker;

public:
	SingleThreadStrategy(const Zone& process_zone) : 
		checker(process_zone)
	{ }

	ClusteringResult proccess(const Points& source, const AppConfig& config) override
	{
		std::vector<Points> zones = checker.zoning(source, config);

		ClusteringResult result;	

		for (const auto& cell_points : zones)
		{
			if (cell_points.empty())
				continue;

			// Якщо в комірці замало точок — вважаємо це фоновим шумом і ігноруємо
			if (cell_points.size() < config.min_points_for_weak)
				continue;

			// Точки пройшли фільтр, зберігаємо їх для майбутньої тріангуляції
			for (const Point& pt : cell_points) 
			{
				result.filtered_geometry.AddPoint(pt);
			}
		}

		std::vector<Point> temp_strong = checker.collect_local_strong(zones, config, 0, zones.size());

		result.cluster_centers = checker.mergeCloseCenters(temp_strong, config);

		return result;
	}
};