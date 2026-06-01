#pragma once

struct AppConfig 
{
    int zones_x = 20;
    int zones_y = 20;
    float radius = 15.0f;
    int min_points_for_strong = 3;
    bool show_grid = true;
    bool regenerate_points = false; // прапорець для тригеру генерації нових точок
    float scale = 1.0f;
};