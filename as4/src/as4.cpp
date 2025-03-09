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

#define DRAW_SCENE \
    do { \
        BeginMode3D(camera);\
                                \
        DrawSphere({0, 0, 0}, 1.0f, RED);\
        DrawGrid(10, 1.0f);\
        DrawSphere(camera.target, 1.0f, BLUE);\
    EndMode3D();\
    } while(0)
    


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

        Vector2 window_main_dim = {400, 400};

        int window_main = InitWindowPro(window_main_dim.x, window_main_dim.y, window_title.c_str(), window_main_flags);
        int window_1 = InitWindowPro(400, 400, "Window 1", window_main_flags);

        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================

        Camera3D camera = { 0 };
        camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };  // Camera position
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };       // Looking at the origin
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };           // Y-axis is up
        camera.fovy = 45.0f;                                  // Field of view (in degrees)

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

        Vector3 camera_base_target = {0, 0, 0};
        Vector3 camera_base_position = {10, 10, 10};
        float camera_base_fov = 45.0f;

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));  
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));  
        Vector3 up = camera.up; 

        Vector2 screen_dim = {GetMonitorWidth(0), GetMonitorHeight(0)};
        Vector2 screen_center = {screen_dim.x/2, screen_dim.y/2};

        SetTargetFPS(DRAW_FPS);

    while (active_game){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================

            SetActiveWindowContext(window_main);
            active_game = !WindowShouldClose();
            SetActiveWindowContext(window_1);
            active_game = active_game || !WindowShouldClose();

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

            UpdateCamera(&camera, CAMERA_PERSPECTIVE);
            
            Matrix view_matrix = MatrixLookAt(camera.position, camera.target, camera.up);
            float camera_distance = Vector3Distance(camera.position, camera.target);
            int diameter_width = 2 * tan(camera.fovy * DEG2RAD * 0.5) * camera_distance;

            int screenWidth = GetMonitorWidth(0);
            int screenHeight = GetMonitorHeight(0);

            SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "cameraPos"), &camera.position, SHADER_UNIFORM_VEC3);
            SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenWidth"), &screenWidth, SHADER_UNIFORM_INT);
            SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenHeight"), &screenHeight, SHADER_UNIFORM_INT);  
            SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "view"), view_matrix);
            SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "projection"), MatrixPerspective(camera.fovy, (double)GetScreenWidth()/(double)GetScreenHeight(), 0.1, 1000.0));


            // ===========================================================
            // Render Block 
            // ===========================================================

                // ===========================================================
                // Window MAIN Draw
                // ===========================================================
            
                SetActiveWindowContext(window_main);

                Vector2 window_main_dim = {GetScreenWidth(), GetScreenHeight()};
                Vector2 window_coords = GetWindowPosition();
                Vector2 window_center = Vector2Add(GetWindowPosition(),Vector2Multiply(window_main_dim, {0.5, 0.5}));
                Vector2 window_offset = Vector2Subtract(window_center, screen_center);

                Vector2 normalized_position = Vector2Divide(window_offset, screen_center);
                normalized_position.y *= -1;

                Vector3 world_offset = {normalized_position.x * diameter_width, normalized_position.y * diameter_width, 0};
                float reduction_factor = 1.0f / (1.0f + Vector2Length(normalized_position));

                std::cout << "Normalized Position: " << normalized_position.x << " " << normalized_position.y << " ";
                std::cout << "Window Center: " << window_center.x << " " << window_center.y << " ";
                std::cout << "Window Offset: " << window_offset.x << " " << window_offset.y << " ";
                std::cout << "World Offset: " << world_offset.x << " " << world_offset.y << " " << world_offset.z << std::endl;

                camera.position = Vector3Add(camera_base_position, world_offset);

                Vector3 camera_translation = Vector3Add(
                    Vector3Add(
                        Vector3Scale(right, world_offset.x),
                        Vector3Scale(up, world_offset.y)
                    ),
                    Vector3Scale(forward, world_offset.z)
                );

                camera.target = Vector3Add(camera_base_target, camera_translation);
                camera.position = Vector3Add(camera_base_position, camera_translation);
                camera.fovy = camera_base_fov * reduction_factor;


                //camera.fovy = 2.0f * atan(reduction_ratio * tan(camera_base_fov * DEG2RAD * 0.5)) * RAD2DEG;
   

                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DRAW_SCENE;

                EndDrawing();

                camera.fovy = camera_base_fov;
                camera.target = camera_base_target;
                camera.position = camera_base_position;

                // ===========================================================
                // Window 1 Draw
                // ===========================================================
                
                    SetActiveWindowContext(window_1);

                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        DRAW_SCENE;

                        float rectX = (window_coords.x/screen_dim.x) * GetScreenWidth();
                        float rectY = (window_coords.y/screen_dim.y) * GetScreenHeight();
                        float rectWidth = window_main_dim.x / screen_dim.x * GetScreenWidth();
                        float rectHeight = window_main_dim.y / screen_dim.x * GetScreenHeight();

                        DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, RED);

                    EndDrawing();
                

                

    }
    
    //Clean-up
    UnloadShader(raymarching_shader);

    SetActiveWindowContext(window_main);
    CloseWindow();

    SetActiveWindowContext(window_1);
    CloseWindow();


    return 0;
}