#pragma once

#include "AppConfig.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h> // або інший бекенд, який ти використовуєш

class GuiManager
{
public:
    GuiManager(SDL_Window* window, SDL_Renderer* renderer)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);
    }

    ~GuiManager() 
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void processEvent(const SDL_Event& event) 
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    void drawInterface(AppConfig& config) 
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Вікно налаштувань алгоритму
        ImGui::Begin("Algorithm Settings");

        ImGui::Text("Grid Parameters:");
        ImGui::SliderInt("Zones X", &config.zones_x, 5, 200);
        ImGui::SliderInt("Zones Y", &config.zones_y, 5, 200);

        ImGui::Separator();
        ImGui::Text("Cluster Parameters:");
        ImGui::SliderFloat("Search Radius", &config.radius, 1.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Merge Radius", &config.merge_radius, 1.0f, 100.0f, "%.1f");
        ImGui::SliderInt("Min Points for strong", &config.min_points_for_strong, 1, 20);
        ImGui::SliderInt("Min Points for weak", &config.min_points_for_weak, 1, 20);

        ImGui::Separator();
        ImGui::Checkbox("Show Grid Lines", &config.show_grid);

        ImGui::Separator();
        ImGui::SliderFloat("Scale factor", &config.scale, 1.0f, 3.0f, "%.1f");

        if (ImGui::Button("Regenerate Points")) 
        {
            config.regenerate_points = true;
        }

        ImGui::End();
        ImGui::Render();
    }

    void render(SDL_Renderer* renderer) 
    {
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
};