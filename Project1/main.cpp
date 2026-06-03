#include "Points.h"
#include "PointsGenerator.h"
#include "Checker.h"
#include "AppConfig.h"
#include <SDL3/SDL.h>
#include "GuiManager.h"
#include "Functions.h"
void SDL_RenderPoint(SDL_Renderer* renderer, Point pt, const AppConfig& config, float point_size = 1);
void SDL_RenderPoints(SDL_Renderer* renderer, const Points& points, const AppConfig& config, float point_size = 1);
void drawGridLines(SDL_Renderer* renderer, const Zone& main_zone, const AppConfig& config);

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

		const bool* keyboard_state = SDL_GetKeyboardState(nullptr);

		// Змінюємо координати камери залежно від стрілочок
		if (keyboard_state[SDL_SCANCODE_LEFT])  config.camera_x += config.camera_speed / config.scale;
		if (keyboard_state[SDL_SCANCODE_RIGHT]) config.camera_x -= config.camera_speed / config.scale;
		if (keyboard_state[SDL_SCANCODE_UP])    config.camera_y += config.camera_speed / config.scale;
		if (keyboard_state[SDL_SCANCODE_DOWN])  config.camera_y -= config.camera_speed / config.scale;

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
			drawGridLines(renderer, mainSurface, config);
		}

		SDL_RenderPoints(renderer, points, config, 1.5);
		SDL_RenderPoints(renderer, weakAvg, config, 2);

		SDL_SetRenderScale(renderer, 1, 1);
		gui.render(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void SDL_RenderPoint(SDL_Renderer* renderer, Point pt, const AppConfig& config, float point_size)
{
	SDL_SetRenderDrawColor(renderer, pt.col.R, pt.col.G, pt.col.B, pt.col.A);

	SDL_FPoint screen_pos = Camera::WorldToScreen(pt.x, pt.y, config);

	if (point_size <= 1)
	{
		SDL_RenderPoint(renderer, screen_pos.x, screen_pos.y);
	}
	else
	{
		SDL_FRect rect{
		screen_pos.x - point_size / 2,
		screen_pos.y - point_size / 2,
		point_size,
		point_size
		};

		SDL_RenderFillRect(renderer, &rect);
	}
}

void SDL_RenderPoints(SDL_Renderer* renderer, const Points& points, const AppConfig& config, float point_size)
{
	for (const auto& pt : points)
	{
		SDL_RenderPoint(renderer, pt, config, point_size);
	}
}

void drawGridLines(SDL_Renderer* renderer, const Zone& main_zone, const AppConfig& config)
{
	double size_x = main_zone.length / config.zones_x;
	double size_y = main_zone.width / config.zones_y;

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

	for (int i = 0; i <= config.zones_x; ++i)
	{
		float world_x = static_cast<float>(main_zone.x + i * size_x);
		
		SDL_FPoint top = Camera::WorldToScreen(world_x, main_zone.y, config);
		SDL_FPoint bottom = Camera::WorldToScreen(world_x, main_zone.y + main_zone.width, config);

		SDL_RenderLine(renderer, top.x, top.y, bottom.x, bottom.y);
	}

	for (int j = 0; j <= config.zones_y; ++j)
	{
		float world_y = static_cast<float>(main_zone.y + j * size_y);
		
		SDL_FPoint left = Camera::WorldToScreen(main_zone.x, world_y, config);
		SDL_FPoint right = Camera::WorldToScreen(main_zone.x+main_zone.length, world_y, config);

		SDL_RenderLine(renderer, left.x, left.y, right.x, right.y);
	}
}