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

//#define WIRE_FRAME_COLOR WHITE //was for an old defunct implementation
#define DEFAULT_SCALE 30

#define LOGIC_SPEED 1
#define DRAW_FPS 60

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
    model.GetTransformedBoundingBox().Draw(); //Feature #4 - Draw Bounding Box (10 points)
    //Thank the Discord. I was scratching my head over this one until I looked at the function
    //implementation and decided to check the Discord. ヾ(≧▽≦*)o
    //I mean, I get *WHY* this exists, but why not just return the transformed matrix by default
    //since that's what appears, and let the "original" matrix be the explicit call instead? 
    // ¯\_(ツ)_/¯
    model.transform = backup;
}
// ===========================================================
// Transformation Lambdas
// ===========================================================
/**
 * @brief Arbitrary translation function that translates on each axis accordingly using a vector.
 * 
 */
auto translate = [](raylib::Vector3 translation) { 
    // Lambda Inception (￣▽￣)"
    return [=](raylib::Matrix& transform) -> raylib::Matrix {
        return transform.Translate(translation);
    };
};
/**
 * @brief Arbitrary scaling function that scales each axis accordingly using a vector.
 * 
 */
auto scale = [](raylib::Vector3 scaling) { 
    // Lambda Inception (￣▽￣)"
    return [=](raylib::Matrix& transform) -> raylib::Matrix {
        return MatrixMultiply(transform, MatrixScale(scaling.x, scaling.y, scaling.z));
    };
};
/**
 * @brief Arbitrary rotation function that rotates (assumedly right hand rule) around the given axis vector.
 * 
 */
auto rotate = [](raylib::Vector3 axis, float angle) {
    // Lambda Inception (￣▽￣)"
    return [=](raylib::Matrix& transform) -> raylib::Matrix {
        return MatrixMultiply(transform, MatrixRotate(axis, angle)); 
    };
};
/**
 * @brief Combines arbitrary number of transformation lambdas into one.
 */
auto combine = [](auto... transformers) {
    // Lambda uh... ㄟ( ▔, ▔ )ㄏ
    return [=](raylib::Matrix& transform) -> raylib::Matrix {
        raylib::Matrix median_transform; //Will in fact flip the hell out if I don't do this (╯°□°）╯︵ ┻━┻
        median_transform = transform; //Not exactly sure why??? My guess is its some goofy addressing hallucinations
        ((median_transform = transformers(median_transform)), ...); //Also this is super cool(￣▽￣)b
        return median_transform;
    };
};

// ===========================================================
// Main Function
// ===========================================================
int main()
{   
    double lastDrawTime = 0; //Deltas for controlling draw+logic rates
    double lastLogicTime = 0;
    double drawDelta = 1.0 / DRAW_FPS; 

    raylib::Window window(800, 600, "CS381 - Assignment 2"); //Feature #1 - Create Titled Window (1 point)
    window.SetState(FLAG_WINDOW_RESIZABLE);
    
    // ===========================================================
    // Model Loading + Default Transforms
    // ===========================================================
    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    auto camera = raylib::Camera({0, 120, 500}, {0, 0, 0}, {0, 1, 0}, 45); //Feature #5 - Draw Camera at (0,120,500) (5 points)
    
    raylib::Model rocket_F6 = raylib::Model("meshes/rocketA.glb");
    rocket_F6.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model rocket_F7 = raylib::Model("meshes/rocketA.glb");
    rocket_F7.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F8 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb"); //Feature #2 - Load Car Model (10 points)
    car_F8.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F9 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb"); 
    car_F9.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    raylib::Model car_F10 = raylib::Model("../../assets/Kenny Car Kit/sedan.glb");
    car_F10.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

    cs381::SkyBox sky("textures/skybox.png");

    while (!window.ShouldClose())
    {
        double currentTime = GetTime();
        
        // ===========================================================
        // Logic Block
        // ===========================================================
        double logicDelta = lastLogicTime - currentTime;

        lastLogicTime = currentTime;


        // ===========================================================
        // Draw Block
        // ===========================================================
        
        window.BeginDrawing();

        if(currentTime - lastDrawTime >= drawDelta)
        {
            window.ClearBackground(BLACK);
            camera.BeginMode();
                sky.Draw();

                auto rocket_F7_transform = combine( //Feature #7 - Rocket Located at (-100, 100, 0), scaled (1,-1,1), and yawed 180 degrees (10 points)
                    rotate({0, 1, 0}, 180*DEG2RAD), // i guess yaw is y-axis here??? 
                    scale({1, -1, 1}),
                    translate({-100, 100, 0})
                );

                auto car_F9_transform = combine( //Feature #9 - Car Located at (200,0,0), and yawed 90 degrees (10 points)
                    rotate({0, 1, 0}, 90*DEG2RAD),
                    translate({200, 0, 0})
                );

                auto car_F10_transform = combine( //Feature #10 - Car Located at (100,100,0), scaled by (1,2,1), and yawed 90 degrees (10 points)
                    rotate({0, 1, 0}, 90*DEG2RAD),
                    scale({1, 2, 1}),
                    translate({100, 100, 0})
                );


                DrawBoundedModel(rocket_F6, translate({0,0,0})); //Feature #6 - Rocket Located at (0,0,0) (10 points)
                DrawBoundedModel(rocket_F7, rocket_F7_transform);
                DrawBoundedModel(car_F8, translate({-200,0,0})); //Feature #8 - Car Located at (-200,0,0) (10 points)
                DrawBoundedModel(car_F9, car_F9_transform);
                DrawBoundedModel(car_F10, car_F10_transform);
                
            camera.EndMode();

            lastDrawTime = currentTime;
        }

        window.EndDrawing(); 
         
        
    }

    return 0;
}

