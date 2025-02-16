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

#define WIRE_FRAME_COLOR WHITE

template<typename T> //Cool type validation!
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
 */
void DrawBoundedModel(raylib::Model& model, Transformer auto transformer)
{   //Feature #3 - Function that Translates Models via Lambda (10 points)
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.GetBoundingBox().Draw(); //Feature #4 - Draw Bounding Box (10 points)
    model.transform = backup;
}

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;

    raylib::Window window(screenWidth, screenHeight, "CS381 - Assignment 2"); //Feature #1 - Create Titled Window (1 point)
    window.SetState(FLAG_WINDOW_RESIZABLE);
    
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    auto camera = raylib::Camera({0, 120, 500}, {0, 0, 0}, {0, 1, 0}, 45); //Feature #5 - Draw Camera at (0,120,500) (5 points)
    
    raylib::Model rocket = raylib::Model("meshes/rocketA.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(30);

    raylib::Model car = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(30);

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        window.BeginDrawing();
            camera.BeginMode();
                window.ClearBackground(BLACK);
                sky.Draw();

                auto position_transform = [](raylib::Matrix& transform) -> raylib::Matrix 
                {
                    return transform.Translate({100, 0, 0});
                };
                
                DrawBoundedModel(rocket, position_transform); 
                DrawBoundedModel(car, position_transform);
                
            camera.EndMode();
        window.EndDrawing();    
    }

    return 0;
}

