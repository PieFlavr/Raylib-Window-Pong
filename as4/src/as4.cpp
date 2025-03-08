/**
 * @file as4.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

#include "raylib-cpp.hpp"	
#include "skybox.hpp"

// DEFAULT RENDER SETTINGs
#define DEFAULT_SCALE 30
#define DRAW_FPS 60
#define LOGIC_FPS 120

// DEFAULT INTERFACE SETTINGs
#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING 2

#define DEFAULT_TITLE "CS381 - Assignment 4"

// DEFAULT GAME SETTINGs
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH 20


template<typename T> //Cool type validation!
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};


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
// Draw Model Functions
// ===========================================================

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
 */
void DrawBoundedModel(raylib::Model& model, Transformer auto transformer)
{   
    raylib::Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    model.Draw({});
    model.GetTransformedBoundingBox().Draw(); 
    model.transform = backup;   
}

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a raylib::Matrix and returns a transformed raylib::Matrix.
 */
void DrawUnboundedModel(raylib::Model& model, Transformer auto transformer)
{   
    raylib::Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    model.Draw({});
    model.transform = backup;   
}

// ===========================================================
// Main Function
// ===========================================================

int main(){

    // ===========================================================
    // Initialization
    // ===========================================================

        // ===========================================================
        // Window Initialization
        // ===========================================================

        std::string window_title = DEFAULT_TITLE;

        raylib::Window window_main(400, 400, window_title); //FOUND OUT WHY THIS CRAP SEGFAULTs
        raylib::Window window_A(800, 600, "Window_A");      //IF YOU TRY TO LOAD A MODEL/CAMERA BEFORE A WINDOW, IT EXPLODES!
        raylib::Window window_B(400, 600, "Window_B");      //SO DON'T DO THAT!

        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================

        raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
        cube.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);

        raylib::Model cow = raylib::Model("../../custom_assets/as2/Cow.glb");   
        cow.transform = raylib::Matrix::Identity().Scale(DEFAULT_SCALE);    // From... https://www.fab.com/listings/fbd8c8b0-fc59-460e-adb8-c7ab8f19a6f9    

        raylib::Camera camera = raylib::Camera(
            {0, 120, 500}, 
            {0, 0, 0}, 
            {0, 1, 0}, 
            45
        );

        // ===========================================================
        // Audio Initialization
        // ===========================================================

        //raylib::AudioDevice defaultDevice;

        // ===========================================================
        // Primary Booleans
        // ===========================================================

        bool active_game = true;
    while (active_game){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================

            active_game = !window_main.ShouldClose();
            active_game = active_game && !window_A.ShouldClose();
            active_game = active_game && !window_B.ShouldClose();

        // ===========================================================
        // Delta Logic Block
        // ===========================================================

            double logicFrameTime = GetFrameTime();
            double logicDelta = logicFrameTime/LOGIC_FPS;



        // ===========================================================
        // Draw Block
        // ==========================================================

            // ===========================================================
            // Draw Logics
            // ===========================================================


            // ===========================================================
            // Render Block 
            // ===========================================================

            window_main.BeginDrawing();
            window_main.ClearBackground(BLACK);

            window_A.BeginDrawing();
            window_A.ClearBackground(BLACK);

            window_B.BeginDrawing();
            window_B.ClearBackground(BLACK);

            camera.BeginMode();


                // ===========================================================
                // Background Draw
                // ===========================================================

                DrawBoundedModel(cow, translate({0, 0, 0}));

                // ===========================================================
                // Midground Draw
                // ===========================================================



                // ===========================================================
                // Foreground Draw
                // ===========================================================

            camera.EndMode();

            window_main.EndDrawing();
            window_A.EndDrawing();
            window_B.EndDrawing();

    }
    


    return 0;
}