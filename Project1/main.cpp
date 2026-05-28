#include "WindowRenderer.h"
#include "Points.h"
#include "PointsGenerator.h"

int main()
{
	int width = 800;
	int height = 600;
	PointsGenerator gen(std::random_device{}());

	Points points = gen.get_points_preset1(width, height);

	WindowRenderer renderer(points);

	if (renderer.Init(L"Points Renderer", width, height)) 
	{
		renderer.Run();
	}

	return 0;
}