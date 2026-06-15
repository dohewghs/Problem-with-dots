#pragma once
#include "IClusteringStrategy.h"
#include "ThreadsManager.h"
#include "Checker.h"

class MultiThreadStrategy : public IClusteringStrategy
{
private:
	ThreadsManager thread_manager;
	Checker checker;

public:
	MultiThreadStrategy(const Zone& proccess_zone, int threads_count = 0) : 
		checker(proccess_zone),
		thread_manager(threads_count)
	{ };

	ClusteringResult proccess(const Points& source, const AppConfig& config) override
	{
		size_t total_cells = config.zones_x * config.zones_y;
		int threads = thread_manager.thread_count();
		size_t chunk_size = total_cells / threads;

		std::vector<Points> zones = checker.zoning(source, config);

		ClusteringResult result;

		std::vector<Points> local_filtered_geometries(threads);
		std::vector<std::vector<Point>> local_strong_centers(threads);

		for (int i = 0; i < threads; ++i)
		{
			size_t start = i * chunk_size;
			size_t end = (i == threads - 1) ? total_cells : start + chunk_size;

			thread_manager.push_task([this, &zones, &config, &local_filtered_geometries, &local_strong_centers, start, end, i]() {
				for (size_t j = start; j < end; ++j)
				{
					if (zones[j].empty())
						continue;

					if (zones[j].size() < config.min_points_for_weak)
						continue;

					for (const Point& pt : zones[j])
					{
						local_filtered_geometries[i].AddPoint(pt);
					}
				}
				local_strong_centers[i] = this->checker.collect_local_strong(zones, config, start, end);
			});
		}

		this->thread_manager.wait_all();

		std::vector<Point> temp_strong;

		for (int i = 0; i < threads; ++i)
		{
			for (const Point& pt : local_filtered_geometries[i])
			{
				result.filtered_geometry.AddPoint(pt);
			}

			for (const Point& pt : local_strong_centers[i])
			{
				temp_strong.push_back(pt);
			}
		}

		result.cluster_centers = checker.mergeCloseCenters(temp_strong, config);

		return result;
	}
};

