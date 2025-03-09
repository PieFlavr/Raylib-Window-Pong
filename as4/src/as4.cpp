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

 // Raytracer includes
//  #include "Raytracer/Raytracer.h"
//  #include "Renderer/display.h"
//  #include "Util/Array.h"
//  #include "raymath.h"

//Ray_Tracer includes


// Multi Window Raylib includes
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>

// DEFAULT RENDER SETTINGs
#define DEFAULT_SCALE 5
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
concept Transformer = requires(T t, Matrix m) {
    { t(m) } -> std::convertible_to<Matrix>;
};


// ===========================================================
// Transformation Lambdas
// ===========================================================
/**
 * @brief Arbitrary translation function that translates on each axis accordingly using a vector.
 * 
 */
auto translate = [](Vector3 translation) { 
    // Lambda Inception (￣▽￣)"
    return [=](Matrix& transform) -> Matrix {
        return MatrixTranslate(translation.x, translation.y, translation.z);
    };
};
/**
 * @brief Arbitrary scaling function that scales each axis accordingly using a vector.
 * 
 */
auto scale = [](Vector3 scaling) { 
    // Lambda Inception (￣▽￣)"
    return [=](Matrix& transform) -> Matrix {
        return MatrixMultiply(transform, MatrixScale(scaling.x, scaling.y, scaling.z));
    };
};
/**
 * @brief Arbitrary rotation function that rotates (assumedly right hand rule) around the given axis vector.
 * 
 */
auto rotate = [](Vector3 axis, float angle) {
    // Lambda Inception (￣▽￣)"
    return [=](Matrix& transform) -> Matrix {
        return MatrixMultiply(transform, MatrixRotate(axis, angle)); 
    };
};
/**
 * @brief Combines arbitrary number of transformation lambdas into one.
 */
auto combine = [](auto... transformers) {
    // Lambda uh... ㄟ( ▔, ▔ )ㄏ
    return [=](Matrix& transform) -> Matrix {
        Matrix median_transform; //Will in fact flip the hell out if I don't do this (╯°□°）╯︵ ┻━┻
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
 * @param transformer A function or lambda that takes a Matrix and returns a transformed Matrix.
 */
void DrawBoundedModel(Model& model, Transformer auto transformer)
{   
    Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    DrawModelWires(model, {0, 0, 0}, 1.0f, WHITE);
    model.transform = backup;
}

/**
 * @brief Draws a model with a transformation applied to it.
 * 
 * @param model The model to be drawn.
 * @param transformer A function or lambda that takes a Matrix and returns a transformed Matrix.
 */
void DrawUnboundedModel(Model& model, Transformer auto transformer)
{   
    Matrix backup = model.transform;   
    model.transform = transformer(backup);  
    DrawModel(model, {0, 0, 0}, 1.0f, WHITE);
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

        //unsigned int window_flags_generic = FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MINIMIZABLE | FLAG_WINDOW_MAXIMIZABLE | FLAG_WINDOW_CLOSEABLE | FLAG_WINDOW_UNDECORATED;
        unsigned int window_main_flags = 0;
        int window_main = InitWindowPro(800, 450, window_title.c_str(), window_main_flags);

        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================

        Camera3D camera_1 = { 0 };
        camera_1.position = (Vector3){ 10.0f, 10.0f, 10.0f };  // Camera position
        camera_1.target = (Vector3){ 0.0f, 0.0f, 0.0f };       // Looking at the origin
        camera_1.up = (Vector3){ 0.0f, 1.0f, 0.0f };           // Y-axis is up
        camera_1.fovy = 45.0f;                                  // Field of view (in degrees)

        // ===========================================================
        // Shader + Other Loading
        // ===========================================================

        SetActiveWindowContext(window_main);

        Shader raymarching_shader = LoadShader(0, "../../custom_assets/as4/raytracing_shader.fs");
        RenderTexture2D targetTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        if (raymarching_shader.id == 0) {
            printf("Error loading raytracing shader!\n");
        } else {
            printf("Raytracing shader loaded successfully.\n");
        }

        // ===========================================================
        // Audio Initialization
        // ===========================================================

        //AudioDevice defaultDevice;

        // ===========================================================
        // Miscallaneous Initialization/Variables
        // ===========================================================

        bool active_game = true;
        SetTargetFPS(DRAW_FPS);

    while (active_game){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================

            SetActiveWindowContext(window_main);
            active_game = !WindowShouldClose();

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

                // ===========================================================
                // Window MAIN Draw
                // ===========================================================

                SetActiveWindowContext(window_main);

                UpdateCamera(&camera_1, CAMERA_FREE);

                Matrix view_matrix = MatrixLookAt(camera_1.position, camera_1.target, camera_1.up);
                int screenWidth = GetScreenWidth();
                int screenHeight = GetScreenHeight();

                SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "cameraPos"), &camera_1.position, SHADER_UNIFORM_VEC3);
                SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenWidth"), &screenWidth, SHADER_UNIFORM_INT);
                SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenHeight"), &screenHeight, SHADER_UNIFORM_INT);  
                SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "view"), view_matrix);
                SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "projection"), MatrixPerspective(camera_1.fovy, (double)GetScreenWidth()/(double)GetScreenHeight(), 0.1, 1000.0));

                BeginDrawing();
                    ClearBackground(RAYWHITE);

                    BeginMode3D(camera_1);

                        DrawSphere({0, 0, 0}, 1.0f, RED);
                        DrawGrid(10, 1.0f);

                    EndMode3D();

                    BeginShaderMode(raymarching_shader);
                        
                        DrawSphere({0, 0, 0}, 1.0f, RED);

                    EndShaderMode();

                EndDrawing();

    }
    
    //Clean-up
    UnloadShader(raymarching_shader);
    CloseWindow();


    return 0;
}