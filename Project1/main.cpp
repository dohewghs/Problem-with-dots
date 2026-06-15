#include <SDL3/SDL.h>
#include "Points.h"
#include "PointsGenerator.h"
#include "AppConfig.h"
#include "GuiManager.h"
#include "Functions.h"
#include "color.h"
#include "SingleThreadStrategy.h"

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
	PointsGenerator gen(std::random_device{}());

	Points points = gen.get_points_preset2(width, height);

	IClusteringStrategy* clustering_strategy = new SingleThreadStrategy(mainSurface);

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
			//points = generateTestPoints();
			config.regenerate_points = false;
		}

		Points working_dots = points; // Скидаємо старі центри перед перерахунком
		
		ClusteringResult after_clustering = clustering_strategy->proccess(working_dots, config);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderScale(renderer, config.scale, config.scale);

		if (config.show_grid) 
		{
			Render::drawGridLines(renderer, mainSurface, config, color(100,100,100,255));
		}

		Render::SDL_RenderPoints(
			renderer,
			working_dots,
			config,
			1.5,
			color(0, 200, 0, 255)
		);

		Render::SDL_RenderPoints(
			renderer, 
			after_clustering.filtered_geometry, 
			config, 
			1.5, 
			color(0,0,0,255)
		);

		Render::SDL_RenderPoints(
			renderer, 
			after_clustering.cluster_centers, 
			config, 
			2, 
			color(255,0,0,255)
		);

		SDL_SetRenderScale(renderer, 1, 1);
		gui.render(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}