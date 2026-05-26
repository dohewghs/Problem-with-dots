#include "WindowRenderer.h"
#include "Points.h"

int main()
{
	Points dots;
	dots.AddPoint(Point(10, 10, 1, color(0, 0, 0)));

	WindowRenderer renderer(dots);

	if (renderer.Init(L"Points Renderer", 800, 600)) 
	{
		renderer.Run();
	}

	return 0;
}