#pragma once
#include <SDL3/SDL_rect.h>
#include "AppConfig.h"

namespace Camera
{
	inline SDL_FPoint WorldToScreen(double world_x, double world_y, const AppConfig& config)
	{
		SDL_FPoint screen;

		screen.x = (static_cast<float>(world_x) + config.camera_x) * config.scale;
		screen.y = (static_cast<float>(world_y) + config.camera_y) * config.scale;
	
		return screen;
	}
}