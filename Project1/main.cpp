#include "WindowRenderer.h"
#include "Points.h"
#include "PointsGenerator.h"
#include "Checker.h"

int main()
{
	PointsGenerator gen(std::random_device{}());

	Points dots;
	dots.AddPoint(Point(100, 100, 1, color(0, 0, 0)));
	dots.AddPoint(Point(200, 100, 1, color(0, 0, 0)));
	dots.AddPoint(Point(150, 300, 1, color(0, 0, 0)));
	dots.AddPoint(Point(430, 270, 1, color(0, 0, 0)));
	
	Points points = gen.genPoints(dots);

	Checker checker(Zone(0, 0, 800, 600));
	checker.checkPoints(points);

	WindowRenderer renderer(points);

	if (renderer.Init(L"Points Renderer", 800, 600)) 
	{
		renderer.Run();
	}

	return 0;
}