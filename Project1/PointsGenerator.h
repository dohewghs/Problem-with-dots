#pragma once
#include <random>
#include "Points.h"

class PointsGenerator
{
private:
	std::mt19937 gen;

	double getRandomNumber()
	{
		std::normal_distribution<double> distr;

		return distr(gen);
	}

	double getNumInRange(double min, double max)
	{
		std::uniform_real_distribution<double> distr(min, max);

		return distr(gen);
	}

	Point changePoint(const Point& point)
	{
		double range = getNumInRange(0.0, 20.0);

		Point res(point);

		res.x = res.x + getNumInRange(-range, range);
		res.y = res.y + getNumInRange(-range, range);

		return res;
	}
public:
	PointsGenerator(unsigned int seed) :
		gen(seed)
	{ }

	Points genPoints(const Points& congestion_points) // точки скупчення
	{
		Points res_points;

		for (const Point& point : congestion_points)
		{
			int count_new_points = static_cast<int>(getNumInRange(0, 20));
			
			for (int i=0; i<count_new_points; ++i)
				res_points.AddPoint(changePoint(point));
		}

		return res_points;
	}
};