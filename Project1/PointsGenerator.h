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
		double range = getNumInRange(0.0, 50.0);

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
			int count_new_points = static_cast<int>(getNumInRange(20, 30));
			
			for (int i=0; i<count_new_points; ++i)
				res_points.AddPoint(changePoint(point));
		}

		return res_points;
	}

	Points get_points_preset1(int width, int height)
	{
		Points dots;
		int n = 20;
		int m = 20;

		int step_y = height / m;
		int step_x = width / n;

		for (int i = 1; i <= n; ++i)
		{
			for (int j = 1; j <= m; ++j)
			{
				dots.AddPoint(Point(step_x * j, step_y * i, 1));
			}
		}
		Points points = this->genPoints(dots);

		return points;
	}

	Points get_points_preset2(int height, int width)
	{
		// height = width
		Points dots;

		// Базові межі нашої тестової зони (ідеальний квадрат)
		double startX = 100.0;
		double startY = 100.0;
		double zoneSize = height; // Ширина і висота

		// 1. Створюємо фоновий випадковий шум (рівномірно по всій зони)
		// Це перевірить, як алгоритм ігнорує поодинокі точки
		for (int i = 0; i < 500; ++i) {
			// std::rand() дає число від 0 до RAND_MAX. Переводимо в діапазон [0, zoneSize]
			double rx = startX + (static_cast<double>(std::rand()) / RAND_MAX) * zoneSize;
			double ry = startY + (static_cast<double>(std::rand()) / RAND_MAX) * zoneSize;
			dots.AddPoint(Point(rx, ry, 0.0));
		}

		// 2. Створюємо 4 чіткі скупчення (кластери) точок.
		// Ми знаємо їхні точні математичні центри:
		// Кластер 1: (200, 200)
		// Кластер 2: (400, 200)
		// Кластер 3: (200, 400)
		// Кластер 4: (400, 400)

		double clusterCenters[4][2] = {
		  {200.0, 200.0},
		  {400.0, 200.0},
		  {200.0, 400.0},
		  {400.0, 400.0}
		};

		// Накидаємо по 50 точок щільно навколо кожного центру (в радіусі 15 пікселів)
		for (int c = 0; c < 4; ++c) {
			double cx = clusterCenters[c][0];
			double cy = clusterCenters[c][1];

			for (int p = 0; p < 50; ++p) {
				// Випадковий зсув від центру в межах [-15, 15]
				double offsetX = ((static_cast<double>(std::rand()) / RAND_MAX) * 30.0) - 15.0;
				double offsetY = ((static_cast<double>(std::rand()) / RAND_MAX) * 30.0) - 15.0;

				dots.AddPoint(Point(cx + offsetX, cy + offsetY, 0.0));
			}
		}

		return dots;
	}
};