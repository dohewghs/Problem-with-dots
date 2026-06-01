#include "Points.h"
#include "PointsGenerator.h"
#include "Checker.h"
#include "AppConfig.h"
#include <SDL3/SDL.h>
#include "GuiManager.h"

void SDL_RenderPoint(SDL_Renderer* renderer, Point pt, float size = 1);
void SDL_RenderPoints(SDL_Renderer* renderer, const Points& points, float size = 1);
void drawGridLines(SDL_Renderer* renderer, const Zone& main_zone, int zones_x, int zones_y);

int main()
{
	int width = 800;
	int height = 800;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("test", width, height, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	AppConfig config;
	GuiManager gui(window, renderer);
	Zone mainSurface(0, 0, width, height);
	Checker checker(mainSurface);
	PointsGenerator gen(std::random_device{}());

	Points points = gen.get_points_preset2(width, height);

	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			gui.processEvent(event); // Передаємо події в ImGui
			if (event.type == SDL_EVENT_QUIT)
			{
				isRunning = false;
			}
		}

		gui.drawInterface(config);

		if (config.regenerate_points) 
		{
			//original_dots = generateTestPoints();
			config.regenerate_points = false;
		}

		Points working_dots = points; // Скидаємо старі центри перед перерахунком
		
		Points weakAvg = checker.weakAverages(points, config);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderScale(renderer, config.scale, config.scale);

		if (config.show_grid) 
		{
			drawGridLines(renderer, mainSurface, config.zones_x, config.zones_y);
		}

		SDL_RenderPoints(renderer, points, 2);
		SDL_RenderPoints(renderer, weakAvg, 2);

		SDL_SetRenderScale(renderer, 1, 1);
		gui.render(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void SDL_RenderPoint(SDL_Renderer* renderer, Point pt, float pointSize)
{
	SDL_SetRenderDrawColor(renderer, pt.col.R, pt.col.G, pt.col.B, pt.col.A);

	if (pointSize <= 1)
	{
		SDL_RenderPoint(renderer, pt.x, pt.y);
	}
	else
	{
		SDL_FRect rect{
		pt.x - pointSize / 2,
		pt.y - pointSize / 2,
		pointSize,
		pointSize
		};

		SDL_RenderFillRect(renderer, &rect);
	}
}

void SDL_RenderPoints(SDL_Renderer* renderer, const Points& points, float point_size)
{
	for (const auto& pt : points)
	{
		SDL_RenderPoint(renderer, pt, point_size);
	}
}

void drawGridLines(SDL_Renderer* renderer, const Zone& main_zone, int zones_x, int zones_y)
{
	double size_x = main_zone.length / zones_x;
	double size_y = main_zone.width / zones_y;

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

	for (int i = 0; i <= zones_x; ++i)
	{
		float x = static_cast<float>(main_zone.x + i * size_x);
		float y1 = static_cast<float>(main_zone.y);
		float y2 = static_cast<float>(main_zone.y + main_zone.width);

		SDL_RenderLine(renderer, x, y1, x, y2);
	}

	for (int j = 0; j <= zones_y; ++j)
	{
		float y = static_cast<float>(main_zone.y + j * size_y);
		float x1 = static_cast<float>(main_zone.x);
		float x2 = static_cast<float>(main_zone.x + main_zone.length);

		SDL_RenderLine(renderer, x1, y, x2, y);
	}
}