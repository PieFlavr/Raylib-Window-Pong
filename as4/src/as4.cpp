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

#include "utils.cpp"

// DEFAULT RENDER SETTINGs
#define DEFAULT_SCALE 5
#define DRAW_FPS 60
#define LOGIC_FPS 120.0f

// DEFAULT INTERFACE SETTINGs
#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING 2
#define DEFAULT_WINDOW_WIDTH 400

#define DEFAULT_TITLE "CS381 - Assignment 4"

// DEFAULT GAME SETTINGs
#define PADDLE_HEIGHT 300
#define PADDLE_WIDTH 30

#define BALL_RADIUS 10

#define WINDOW_VELOCITY 100000.0f
#define GRAVITY 98000000.0f

#define DRAW_3D_SCENE \
    do { \
        BeginMode3D(camera);\
                                \
        DrawSphere({0, 0, 0}, 1.0f, RED);\
        DrawGrid(10, 1.0f);\
        /**DrawSphere(camera.target, 1.0f, BLUE);**/ \
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

        Vector2 window_main_dim = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH};

        int window_main = InitWindowPro(window_main_dim.x, window_main_dim.y, window_title.c_str(), window_main_flags);

        int screenWidth = GetMonitorWidth(0);
        int screenHeight = GetMonitorHeight(0);

        float aspect_ratio = (double)screenWidth / (double)screenHeight;
        std::cout << "Aspect Ratio: " << aspect_ratio << std::endl;

        int scoreboard_window = InitWindowPro(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio, "Window 1", window_main_flags);

        SetActiveWindowContext(window_main);
        SetWindowPosition(screenWidth/2, screenHeight/2);

        SetActiveWindowContext(scoreboard_window);
        SetWindowPosition(screenWidth/2, screenHeight/2);


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

        InitAudioDevice();
        SetMasterVolume(0.5f);

        Music background_music = LoadMusicStream("../../custom_assets/as4/cats_on_mars.mp3");
        PlayMusicStream(background_music);

        // ===========================================================
        // Game "Objects" and Variables
        // ===========================================================

        float drag = 0.9f;
        float gravity = GRAVITY;
        
        Vector2 left_paddle_size = {PADDLE_WIDTH, PADDLE_HEIGHT};
        Vector2 left_paddle_pos = {50, (screenHeight/2) - (PADDLE_HEIGHT/2)};

        Vector2 right_paddle_size = {PADDLE_WIDTH, PADDLE_HEIGHT};
        Vector2 right_paddle_pos = {screenWidth - 50 - PADDLE_WIDTH, (screenHeight/2) - (PADDLE_HEIGHT/2)};

        Vector2 ball_pos = {screenWidth/2, screenHeight/2};
        float ball_radius = BALL_RADIUS;

        Vector2 main_window_velocity = {WINDOW_VELOCITY, WINDOW_VELOCITY};
        Vector2 main_window_coords = GetWindowPosition();

        Vector2 scoreboard_window_velocity = {0.0f, 0.0f};
        Vector2 scoreboard_window_dim = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio};
        Vector2 scoreboard_window_dim_default = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio};
        Vector2 scoreboard_window_coords = GetWindowPosition();

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
            SetActiveWindowContext(scoreboard_window);
            active_game = active_game || !WindowShouldClose();

            // ===========================================================
            // Main Window Positioning Logic
            // ===========================================================
                SetActiveWindowContext(window_main);

                Vector2 window_main_dim = {GetScreenWidth(), GetScreenHeight()};
                Vector2 main_window_center = Vector2Add(GetWindowPosition(),Vector2Multiply(window_main_dim, {0.5, 0.5}));
                Vector2 main_window_offset = Vector2Subtract(main_window_center, screen_center);

                Vector2 normalized_position = Vector2Divide(main_window_offset, screen_center);
                float square_y_normal = main_window_offset.y / screen_center.x;
                normalized_position.y *= -1;

            // ===========================================================
            // Audio Block
            // ===========================================================

            if(IsMusicReady(background_music)){
                //UpdateMusicStream(background_music);
            }

        // ===========================================================
        // Delta Logic Block
        // ===========================================================

            // ===========================================================
            // Main Window Iteration Logic
            // ===========================================================

            double logicFrameTime = GetFrameTime();
            float logicDelta = logicFrameTime/LOGIC_FPS;

            SetActiveWindowContext(window_main);
                main_window_coords.x = main_window_coords.x + (main_window_velocity.x * logicDelta);
                main_window_coords.y = main_window_coords.y + (main_window_velocity.y * logicDelta);

                // std::cout << main_window_coords.x << " " << main_window_coords.y << " ";
                // std::cout << (main_window_coords.x <= 0 && main_window_velocity.x < 0) << " " << (main_window_coords.x + window_main_dim.x >= screenWidth && main_window_velocity.x > 0);  
                // std::cout << " " << main_window_velocity.x << std::endl;

                //My hatred for this code is immeasurable
                //And my day is ruined

                Vector2 main_window_collision = CheckCollisionWithBoundary({main_window_coords.x, main_window_coords.y, window_main_dim.x, window_main_dim.y}, 
                    {0, 0, screenWidth, screenHeight}, main_window_velocity);

                main_window_velocity.x *= main_window_collision.x;
                main_window_velocity.y *= main_window_collision.y;

                // if((main_window_coords.x <= 0 && main_window_velocity.x < 0) || 
                //     (main_window_coords.x + window_main_dim.x >= screenWidth && main_window_velocity.x > 0)){
                //     main_window_velocity.x *= -1.0f;
                // } 
                // if((main_window_coords.y <= 0 && main_window_velocity.y < 0) || 
                //     (main_window_coords.y + window_main_dim.y >= screenHeight && main_window_velocity.y > 0)){
                //     main_window_velocity.y *= -1.0f;
                // }


                main_window_coords.x = std::fmax(0, std::fmin(screenWidth - window_main_dim.x, main_window_coords.x));
                main_window_coords.y = std::fmax(0, std::fmin(screenHeight - window_main_dim.y, main_window_coords.y));

                SetWindowPosition((int)main_window_coords.x, (int)main_window_coords.y);  
                SetWindowSize(window_main_dim.x, window_main_dim.y);

        // ===========================================================
        // Scoreboard Window Iteration Logic
        // ===========================================================

            SetActiveWindowContext(scoreboard_window);
 
                Vector2 scoreboard_window_collision = CheckCollisionWithBoundary({scoreboard_window_coords.x, scoreboard_window_coords.y, scoreboard_window_dim.x, scoreboard_window_dim.y}, 
                {0, 0, screenWidth, screenHeight}, scoreboard_window_velocity);

                Vector2 mouse_pos = GetMousePosition();

                if(IsKeyDown(KEY_SPACE)){
                    scoreboard_window_dim.x = lerp(scoreboard_window_dim.x, scoreboard_window_dim_default.x*1.3, 0.1f);
                    scoreboard_window_dim.y = lerp(scoreboard_window_dim.y, scoreboard_window_dim_default.y*01.3, 0.1f);

                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        scoreboard_window_coords.x = lerp(scoreboard_window_coords.x, scoreboard_window_coords.x + mouse_pos.x - scoreboard_window_dim.x/2, 0.5f);
                        scoreboard_window_coords.y = lerp(scoreboard_window_coords.y, scoreboard_window_coords.y + mouse_pos.y - scoreboard_window_dim.y/2, 0.5f);
                    } 

                    scoreboard_window_coords.x = std::fmax(0, std::fmin(screenWidth - scoreboard_window_dim.x, scoreboard_window_coords.x));
                    scoreboard_window_coords.y = std::fmax(0, std::fmin(screenHeight - scoreboard_window_dim.y, scoreboard_window_coords.y));
 
                } else {
                    scoreboard_window_coords.x = scoreboard_window_coords.x + (scoreboard_window_velocity.x * logicDelta);
                    scoreboard_window_coords.y = scoreboard_window_coords.y + (scoreboard_window_velocity.y * logicDelta); 

                    scoreboard_window_velocity.x *= scoreboard_window_collision.x;
                    scoreboard_window_velocity.y *= scoreboard_window_collision.y;
                
                    scoreboard_window_velocity.y += gravity * logicDelta;
                    scoreboard_window_velocity = Vector2Multiply(scoreboard_window_velocity, {drag, drag});

                    scoreboard_window_dim.x = lerp(scoreboard_window_dim.x, scoreboard_window_dim_default.x, 0.1f);
                    scoreboard_window_dim.y = lerp(scoreboard_window_dim.y, scoreboard_window_dim_default.y, 0.1f);

                    scoreboard_window_coords.x = std::fmax(0, std::fmin(screenWidth - scoreboard_window_dim.x, scoreboard_window_coords.x));
                    scoreboard_window_coords.y = std::fmax(0, std::fmin(screenHeight - scoreboard_window_dim.y, scoreboard_window_coords.y));
                }

                SetWindowPosition((int)scoreboard_window_coords.x, (int)scoreboard_window_coords.y);
                SetWindowSize(scoreboard_window_dim.x, scoreboard_window_dim.y);
                    

        // ===========================================================
        // Draw Block
        // ==========================================================
            
            // ===========================================================
            // Draw Logics
            // ===========================================================

            SetActiveWindowContext(window_main);

            UpdateCamera(&camera, CAMERA_PERSPECTIVE);
            
            Matrix view_matrix = MatrixLookAt(camera.position, camera.target, camera.up);
            float camera_distance = Vector3Distance(camera.position, camera.target);
            int diameter_width = 2 * tan(camera.fovy * DEG2RAD * 0.5) * camera_distance;

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
            
                    // ===========================================================
                    // 3D Background Logic
                    // ===========================================================

                        SetActiveWindowContext(window_main);

                        Vector3 world_offset = {normalized_position.x * diameter_width, normalized_position.y * diameter_width, 0};
                        float reduction_factor = 1.0f / (1.0f + Vector2Length({normalized_position.x, square_y_normal}));

                        // std::cout << "Normalized Position: " << normalized_position.x << " " << normalized_position.y << " ";
                        // std::cout << "Window Center: " << main_window_center.x << " " << main_window_center.y << " ";
                        // std::cout << "Window Offset: " << main_window_offset.x << " " << main_window_offset.y << " ";
                        // std::cout << "World Offset: " << world_offset.x << " " << world_offset.y << " " << world_offset.z << std::endl;

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

                    do{
                        // ===========================================================
                        // Game Draw Logic
                        // ===========================================================
            
                        Vector2 left_paddle_relative_pos = GetRelativePosition(left_paddle_pos, window_main, window_main);
                        Vector2 right_paddle_relative_pos = GetRelativePosition(right_paddle_pos, window_main, window_main);
                        Vector2 ball_relative_pos = GetRelativePosition(ball_pos, window_main, window_main);
                    
                        // ===========================================================
                        // ACTUAL RENDERING
                        // ===========================================================

                        BeginDrawing();

                        ClearBackground(RAYWHITE);
                        DRAW_3D_SCENE;

                        // std::cout << "Screen Dim: " << screenWidth << " " << screenHeight << std::endl;
                        // std::cout << "Paddle Pos: " << left_paddle_pos.x << " " << left_paddle_pos.y;
                        // std::cout << " Paddle Size: " << left_paddle_size.x << " " << left_paddle_size.y;
                        // std::cout << " Ball Pos: " << ball_pos.x << " " << ball_pos.y;
                        // std::cout << " Ball Radius: " << ball_radius << std::endl;

                        DrawRectangleLines(left_paddle_relative_pos.x, left_paddle_relative_pos.y, left_paddle_size.x, left_paddle_size.y, RED);
                        DrawRectangleLines(right_paddle_relative_pos.x, right_paddle_relative_pos.y, right_paddle_size.x, right_paddle_size.y, BLUE);
                        DrawCircleV(ball_relative_pos, ball_radius, GREEN);

                        EndDrawing();
                    } while (0);
                    

                    camera.fovy = camera_base_fov;
                    camera.target = camera_base_target;
                    camera.position = camera_base_position;

                // ===========================================================
                // Window 1 Draw
                // ===========================================================
                
                    SetActiveWindowContext(scoreboard_window);

                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        DRAW_3D_SCENE;

                        float rectX = (main_window_coords.x/screen_dim.x) * GetScreenWidth();
                        float rectY = (main_window_coords.y/screen_dim.y) * GetScreenHeight();
                        float rectWidth = window_main_dim.x / screen_dim.x * GetScreenWidth();
                        float rectHeight = window_main_dim.y / screen_dim.y * GetScreenHeight();

                        DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, RED);

                    EndDrawing();
                

                

    }
    
    //Clean-up

    UnloadMusicStream(background_music);
    UnloadShader(raymarching_shader);

    SetActiveWindowContext(window_main);
    CloseWindow();

    SetActiveWindowContext(scoreboard_window);
    CloseWindow();


    return 0;
}