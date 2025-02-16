/**
 * @file as2.cpp
 * @author Lucas Pinto
 * @brief 
 * @version 0.1
 * @date 2025-02-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

#include "raylib-cpp.hpp"
#include "skybox.hpp"

template<typename T> //Cool type validation!
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

/**
 * @brief 
 * 
 * @param model 
 * @param transformer 
 */
void DrawBoundedModel(raylib::Model& model, Transformer auto transformer)
{
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.transform = backup;
}

/**
 * @brief 
 * 
 * @return int 
 */
int main()
{
    int screenWidth = 800;
    int screenHeight = 600;

    raylib::Window window(screenWidth, screenHeight, "CS381 - Assignment 2");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    auto camera = raylib::Camera({0, 120, -500}, {0, 0, 0}, {0, 1, 0}, 45);
    
    raylib::Model rocket = raylib::Model("meshes/rocketA.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(30);
    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        window.BeginDrawing();
            camera.BeginMode();
                window.ClearBackground(BLACK);
                sky.Draw();

                auto transformer = [](raylib::Matrix& transform) -> raylib::Matrix 
                {
                    return transform.Translate({100, 0, 0});
                };
                
                // cube.Draw({100, 0, 0});
                DrawBoundedModel(rocket, transformer);
                // rocket.Draw({});
            camera.EndMode();
        window.EndDrawing();    
    }

    return 0;
}

