#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "AppConfig.h"
#include "Points.h"
#include "color.h"
#include "Zone.h"

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

namespace GridMath
{
	inline size_t index_1D(size_t i, size_t j, size_t n)
	{
		return i + j * n;
	}

	inline std::pair<size_t, size_t> index_2D(size_t index_1D, size_t n)
	{
		size_t j = index_1D / n;
		size_t i = index_1D % n;

		return { i,j };
	}
}

namespace Render
{
	inline void SDL_RenderPoint(SDL_Renderer* renderer, Point pt, const AppConfig& config, float point_size, const color& col)
	{
		SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, col.A);

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

	inline void SDL_RenderPoints(SDL_Renderer* renderer, const Points& points, const AppConfig& config, float point_size, const color& col)
	{
		for (const auto& pt : points)
		{
			SDL_RenderPoint(renderer, pt, config, point_size, col);
		}
	}

	inline void drawGridLines(SDL_Renderer* renderer, const Zone& main_zone, const AppConfig& config, const color& col)
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
			SDL_FPoint right = Camera::WorldToScreen(main_zone.x + main_zone.length, world_y, config);

			SDL_RenderLine(renderer, left.x, left.y, right.x, right.y);
		}
	}
}