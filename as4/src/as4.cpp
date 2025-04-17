/**
 * @file as4.cpp
 * @author Lucas Pinto
 * @brief A pong game within a pong game where the player controls a paddle to hit a ball while the windows themselves also bounce around the screen.
 *       Created for Assignment 4 of CS 381, and modified for public release. Coded in "ad hoc" or strictly no architecture, with workarounds to 
 *      still reasonably implement the game logic.
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 //Stupid windows macro
 #ifdef DrawText
 #undef DrawText
 #endif

// Multi Window Raylib includes
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <fstream>

// Organizational Inncludes
#include "draw_macros.h"
#include "utils.cpp"

// AUDIO INCLUDES
#include "miniaudio.h"
#include "input_audio.cpp"

// Other include
#include <chrono>

// DEFAULT RENDER SETTINGs
#define DEFAULT_SCALE 5
#define DRAW_FPS 300
#define LOGIC_FPS 120.0f

// DEFAULT INTERFACE SETTINGs
#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING 2
#define DEFAULT_WINDOW_WIDTH 400
#define MAIN_WINDOW_WIDTH 600

// TERMINAL ASCII VISUALIZER SETTINGS
#define AUDIO_LINE_LENGTH 200
#define AUDIO_NUM_LINES 8
#define VISUALIZER_SPEED_SCALE 100.0f

#define DEFAULT_TITLE "Raylib Window Pong"

// DEFAULT GAME SETTINGs
#define PADDLE_HEIGHT 300
#define PADDLE_WIDTH 30
#define PADDLE_SPEED 15.0f

#define BALL_SPEED 8.0f
#define BALL_RADIUS 10

#define WINDOW_VELOCITY 10.0f
#define GRAVITY 9.8f



template<typename T> //Cool type validation!
concept Transformer = requires(T t, Matrix m) {
    { t(m) } -> std::convertible_to<Matrix>;
};

// ===========================================================
// Draw Model Functions
// ===========================================================
#pragma region Draw Model Functions
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
#pragma endregion

// ===========================================================
// Main Function
// ===========================================================

int main(){

    // ===========================================================
    // Initialization
    // ===========================================================
    
        // ===========================================================
        // File Loading + Reading
        // ===========================================================
        #pragma region File Loading + Reading
        
        // Open the leaderboard file in read/write mode
        std::fstream leaderboard("leaderboard.txt");
        if(!leaderboard.is_open()){ // Check if the file failed to open
            // If the file doesn't exist, create a new leaderboard file
            std::ofstream new_leaderboard("leaderboard.txt");
            if(!new_leaderboard.is_open()){ // Check if the new file creation failed
            std::cout << "Error creating leaderboard file!" << std::endl;
            return -1; // Exit with an error code
            } else {
            std::cout << "Leaderboard file created successfully." << std::endl;
            new_leaderboard.close(); // Close the newly created file
            }
            // Reopen the leaderboard file after creating it
            leaderboard.open("leaderboard.txt");
        } else {
            std::cout << "Leaderboard file opened successfully." << std::endl;
        }

        // Initialize variables to store the top 5 leaderboard entries
        std::string rank_1, rank_2, rank_3, rank_4, rank_5;
        std::string line; 
        // Read the leaderboard file line by line
        while(std::getline(leaderboard, line)){
            // Assign each line to the appropriate rank variable
            if(rank_1.empty()){
            rank_1 = line;
            } else if(rank_2.empty()){
            rank_2 = line;
            } else if(rank_3.empty()){
            rank_3 = line;
            } else if(rank_4.empty()){
            rank_4 = line;
            } else if(rank_5.empty()){
            rank_5 = line;
            }
        }
        // Extract the names and times from the leaderboard entries
        // Each entry is expected to be in the format: "name time"

        // Extract the names (substring before the space)
        std::string rank_1_name = rank_1.substr(0, rank_1.find(" "));
        std::string rank_2_name = rank_2.substr(0, rank_2.find(" "));
        std::string rank_3_name = rank_3.substr(0, rank_3.find(" "));
        std::string rank_4_name = rank_4.substr(0, rank_4.find(" "));
        std::string rank_5_name = rank_5.substr(0, rank_5.find(" "));

        // Extract the times (substring after the space and convert to double)
        double rank_1_time = std::stod(rank_1.substr(rank_1.find(" ") + 1));
        double rank_2_time = std::stod(rank_2.substr(rank_2.find(" ") + 1));
        double rank_3_time = std::stod(rank_3.substr(rank_3.find(" ") + 1));
        double rank_4_time = std::stod(rank_4.substr(rank_4.find(" ") + 1));
        double rank_5_time = std::stod(rank_5.substr(rank_5.find(" ") + 1));

        #pragma endregion

        // ===========================================================
        // Window Initialization
        // ===========================================================
        #pragma region Window Initialization
        
        std::string window_title = DEFAULT_TITLE;

        // Initialize main window flags
        unsigned int window_main_flags = 0;

        // Set dimensions for the main window
        Vector2 main_window_dim = {MAIN_WINDOW_WIDTH, MAIN_WINDOW_WIDTH};

        // Initialize the main window
        int window_main = InitWindowPro(main_window_dim.x, main_window_dim.y, window_title.c_str(), window_main_flags);

        // Get screen dimensions
        int screenWidth = GetMonitorWidth(0);
        int screenHeight = GetMonitorHeight(0);

        // Calculate aspect ratio for the scoreboard window
        float aspect_ratio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

        // Initialize additional windows
        int scoreboard_window = InitWindowPro(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio, "Scoreboard", window_main_flags);
        int left_paddle_window = InitWindowPro(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH, "Left Paddle View", window_main_flags);
        int right_paddle_window = InitWindowPro(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH, "Right Paddle View", window_main_flags);

        // Set active context and configure main window
        SetActiveWindowContext(window_main);
        SetTargetFPS(DRAW_FPS);
        SetWindowPosition(screenWidth/2, screenHeight/2);

        // Set active context and configure scoreboard window
        SetActiveWindowContext(scoreboard_window);
        SetTargetFPS(DRAW_FPS);
        SetWindowPosition(screenWidth/2, screenHeight/2);

        // Set active context and configure left paddle window
        SetActiveWindowContext(left_paddle_window);
        SetTargetFPS(DRAW_FPS);
        SetWindowPosition(screenWidth/2, screenHeight/2);

        // Set active context and configure right paddle window
        SetActiveWindowContext(right_paddle_window);
        SetTargetFPS(DRAW_FPS);
        SetWindowPosition(screenWidth/2, screenHeight/2);
        
        #pragma endregion

        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================
        #pragma region Model Loading + Default Transforms
        
        // Load a cube model from a generated mesh
        Model cube = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));   

        // Load a cow model from a GLB file
        Model cow = LoadModel("../../custom_assets/as2/Cow.glb");

        // Load a texture for the cow model
        Texture2D cow_texture = LoadTexture("../../custom_assets/as2/cow_color_1.png");

        // Assign the loaded texture to the cow model's material
        cow.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = cow_texture;

        // Initialize the 3D camera
        Camera3D camera = { 0 };
        camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };  // Camera position
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };       // Looking at the origin
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };           // Y-axis is up
        camera.fovy = 45.0f;                                 // Field of view (in degrees)

        #pragma endregion

        // ===========================================================
        // Shader + Other Loading
        // ===========================================================
        #pragma region Shader + Other Loading

        SetActiveWindowContext(window_main);

        // Gave up doesn't want to work consistently... maybe later!
        // Shader raymarching_shader = LoadShader(NULL, "../../custom_assets/as4/raytracing_shader.fs");
        // RenderTexture2D targetTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        // if (raymarching_shader.id == 0) {
        //     printf("Error loading raytracing shader!\n");
        // } else {
        //     printf("Raytracing shader loaded successfully.\n");
        // }

        #pragma endregion

        // ===========================================================
        // Audio Initialization
        // ===========================================================
        #pragma region Audio Initialization

            // Initialize the audio device for playback
            InitAudioDevice();
            SetMasterVolume(0.5f); // Set the master volume to 50%

            // Load music streams for different game states
            Music game_state_0_music = LoadMusicStream("../../custom_assets/as4/lucky_star.mp3"); // Menu music, from https://youtu.be/1hbHgZDB95o?si=YvHRP71iqUQcIBFy 
            Music game_state_1_music = LoadMusicStream("../../custom_assets/as4/cats_on_mars.mp3"); // Gameplay music, from https://youtu.be/97xfV6yXcrk?si=uDQGLkleoicX_xSb 
            Music game_state_3_music = LoadMusicStream("../../custom_assets/as4/determination.mp3"); // Game over music, from https://youtu.be/W1i4mTyidOc?si=mzxLFXQke-5vfmj2 
            PlayMusicStream(game_state_0_music); // Start playing the menu music

            // Configure the audio capture device
            ma_device_config device_config = ma_device_config_init(ma_device_type_capture);
            device_config.capture.format = ma_format_f32; // Set the audio format to 32-bit float
            device_config.capture.channels = 1; // Use a single audio channel (mono)
            device_config.sampleRate = 44100; // Set the sample rate to 44.1 kHz
            device_config.dataCallback = data_callback; // Callback function for audio data processing

            // Initialize a string to store the visualizer's bar encoding
            std::string bar_encoding = "";

            // Declare the audio capture device
            ma_device device;

            // Initialize the audio capture device
            if(ma_device_init(NULL, &device_config, &device) != MA_SUCCESS){
                std::cout << "Output audio device falied to initialized!" << std::endl; // Error message if initialization fails
                return -1;
            }
            // Start the audio capture device
            if (ma_device_start(&device) != MA_SUCCESS) {
                std::cout << "Capture audio device failed to initialize!" << std::endl; // Error message if starting the device fails
                return -1;
            }

            // Initialize the visualizer accumulator for timing
            float visualizer_accumulator = 0.0f;

        #pragma endregion

        // ===========================================================
        // Game "Objects" and Variables
        // ===========================================================
        #pragma region Game Variables

            int game_state = 0; // 0 - Menu | 1 - Playing | 2 - ??? | 3 - Game Over

            // Drag coefficient for window movement
            float drag = 0.8f;

            // Gravity affecting window movement
            float gravity = GRAVITY;

            // Time scale for game speed adjustments
            float time_scale = 1.0f;

            // Left paddle dimensions and initial position
            Vector2 left_paddle_size = {PADDLE_WIDTH, PADDLE_HEIGHT};
            Vector2 left_paddle_pos = {50, (screenHeight/2) - (PADDLE_HEIGHT/2)};

            // Right paddle dimensions and initial position
            Vector2 right_paddle_size = {PADDLE_WIDTH, PADDLE_HEIGHT};
            Vector2 right_paddle_pos = {screenWidth - 50 - PADDLE_WIDTH, (screenHeight/2) - (PADDLE_HEIGHT/2)};

            // Ball position, velocity, and radius
            Vector2 ball_pos = {screenWidth/2, screenHeight/2};
            Vector2 ball_velocity = {BALL_SPEED, BALL_SPEED};
            float ball_radius = BALL_RADIUS;

            // Main window velocity and position
            Vector2 main_window_velocity = {WINDOW_VELOCITY, WINDOW_VELOCITY};
            Vector2 main_window_pos = GetWindowPosition();

            // Scoreboard window velocity, dimensions, and position
            Vector2 scoreboard_window_velocity = {0.0f, 0.0f};
            Vector2 scoreboard_window_dim = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio};
            Vector2 scoreboard_window_dim_default = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH/aspect_ratio};
            Vector2 scoreboard_window_coords = GetWindowPosition();

            // Left paddle window position, velocity, and dimensions
            Vector2 left_paddle_window_pos = {10, (screenHeight/2) - (PADDLE_HEIGHT/2)};
            Vector2 left_paddle_window_velocity = {0.0f, 0.0f};
            Vector2 left_paddle_window_dim = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH};

            // Right paddle window position, velocity, and dimensions
            Vector2 right_paddle_window_pos = {screenWidth - 10 - PADDLE_WIDTH - DEFAULT_WINDOW_WIDTH, (screenHeight/2) - (PADDLE_HEIGHT/2)};
            Vector2 right_paddle_window_velocity = {0.0f, 0.0f};
            Vector2 right_paddle_window_dim = {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_WIDTH};

        #pragma endregion

        // ===========================================================
        // Miscallaneous Initialization/Variables
        // ===========================================================
        #pragma region Miscallaneous Initialization/Variables
            bool active_game = true;
            double total_time = 0.0f;
            double timer = 30.0f;

            Vector3 camera_base_target = {0, 0, 0};
            Vector3 camera_base_position = {10, 10, 10};
            float camera_base_fov = 45.0f;

            Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));  
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));  
            Vector3 up = camera.up; 

            Vector2 screen_dim = {GetMonitorWidth(0), GetMonitorHeight(0)};
            Vector2 screen_center = {screen_dim.x/2, screen_dim.y/2};

            SetTargetFPS(DRAW_FPS);
        #pragma endregion

    while (active_game){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================
        #pragma region Non-Delta Logic Block
            // Check for either scoreboard or main window
            SetActiveWindowContext(window_main);
            active_game = !WindowShouldClose();
            SetActiveWindowContext(scoreboard_window);
            active_game = active_game || !WindowShouldClose();
            

            // ===========================================================
            // General Game State
            // ===========================================================
            #pragma region General Game State

                if(game_state == 0){
                    if(IsKeyPressed(KEY_ENTER)){
                        game_state = 1;

                        float angle = (rand() % 4) * 90.0f + 45.0f;
                        angle *= DEG2RAD;
                        ball_pos = {static_cast<float>(screenWidth)/2.0f, static_cast<float>(screenHeight)/2.0f};
                        ball_velocity = {BALL_SPEED * cos(angle), BALL_SPEED * sin(angle)};

                        main_window_velocity.x = (rand() % 2 == 0 ? 1 : -1) * WINDOW_VELOCITY;
                        main_window_velocity.y = (rand() % 2 == 0 ? 1 : -1) * WINDOW_VELOCITY;

                        StopMusicStream(game_state_0_music);
                        PlayMusicStream(game_state_1_music);
                    }
                    // Menu State
                } else if(game_state == 1){ // Playing State
                    gravity = -GRAVITY;
                    timer -= GetFrameTime(); 
                    total_time += GetFrameTime();

                    if(timer <= 0){
                        timer = 0;
                        game_state = 3;

                        StopMusicStream(game_state_1_music);
                        PlayMusicStream(game_state_3_music);
                    }
                } else if(game_state == 2){
                    // ??? State
                } else if(game_state == 3){// Game Over State
                   gravity = GRAVITY*0.5f;
                   if(IsKeyPressed(KEY_ENTER)){
                       game_state = 0;
                        timer = 30.0f;
                        total_time = 0.0f;
                        PlayMusicStream(game_state_0_music);
                        StopMusicStream(game_state_3_music);
                   }
                }

                #pragma endregion

            // ===========================================================
            // Audio Block
            // ===========================================================
            #pragma region Audio Block

                if(game_state == 0){//https://youtu.be/1hbHgZDB95o?si=ceS8gfdORZbHu4tL
                    UpdateMusicStream(game_state_0_music);
                } else if(game_state == 1){//https://youtu.be/97xfV6yXcrk?si=MhnQx-91z3umuxd3
                    UpdateMusicStream(game_state_1_music);
                } else if(game_state == 3){//https://www.youtube.com/watch?v=h1wSPmlZV-w
                    UpdateMusicStream(game_state_3_music);
                }

                float LOUD = calculate_loud();
                
                if(game_state != 3){ //Game Over, no visualizer

                    visualizer_accumulator += GetFrameTime();
                    if(visualizer_accumulator >= (1.0f/((float)LOGIC_FPS * VISUALIZER_SPEED_SCALE))){

                        bar_encoding += generateEncodedString(LOUD, 0.0f, 0.1f, AUDIO_NUM_LINES); 
                        if(bar_encoding.size() > AUDIO_LINE_LENGTH*AUDIO_NUM_LINES){
                            bar_encoding = bar_encoding.substr(bar_encoding.size() - AUDIO_LINE_LENGTH*AUDIO_NUM_LINES);
                            renderEncodedString(bar_encoding, AUDIO_LINE_LENGTH, AUDIO_NUM_LINES);
                        }
                    }
                }

            #pragma endregion
        #pragma endregion
        
        // ===========================================================
        // Delta Logic Block
        // ===========================================================
            #pragma region Delta Logic Block

                double logicFrameTime = GetFrameTime();
                float logicDelta = logicFrameTime  * (float)LOGIC_FPS;
                
            // ===========================================================
            // Time Scale Logic
            // ===========================================================
                #pragma region Time Scale Logic

                if(game_state == 1){
                    if(LOUD > 0.05f){
                        time_scale = lerp(time_scale, 1.0f/LOUD, 0.1f);
                    } else {
                        time_scale = lerp(time_scale, 1.0f, 0.1f);
                    }
                } else if (game_state == 3){
                    time_scale = lerp(time_scale, 0.3f, 0.5f);
                } else {
                    time_scale = lerp(time_scale, 1.0f, 0.1f);
                }
                logicDelta = logicDelta * time_scale;

                #pragma endregion
            
            // ===========================================================
            // Pong Controls
            // ===========================================================
                #pragma region Pong Controls

                if(game_state == 1){
                    if(IsKeyDown(KEY_W)){
                        left_paddle_pos.y -= PADDLE_SPEED * logicDelta;
                    }
                    if(IsKeyDown(KEY_S)){
                        left_paddle_pos.y += PADDLE_SPEED * logicDelta;
                    }
    
                    if(IsKeyDown(KEY_UP)){
                        right_paddle_pos.y -= PADDLE_SPEED * logicDelta;
                    }
                    if(IsKeyDown(KEY_DOWN)){
                        right_paddle_pos.y += PADDLE_SPEED * logicDelta;
                    }
                }

                left_paddle_pos.y = std::fmax(0, std::fmin(screenHeight - left_paddle_size.y, left_paddle_pos.y));
                right_paddle_pos.y = std::fmax(0, std::fmin(screenHeight - right_paddle_size.y, right_paddle_pos.y));

            #pragma endregion
            
            // ===========================================================
            // Pong Logic
            // ===========================================================
                #pragma region Pong Logic

                if(game_state == 1){
                    ball_pos.x += ball_velocity.x * logicDelta;
                    ball_pos.y += ball_velocity.y * logicDelta;

                    Vector2 ball_collision = CheckCollisionBoundary({ball_pos.x, ball_pos.y, ball_radius, ball_radius}, 
                        {0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, ball_velocity);

                    if(ball_collision.x < 0.0f){ //Side Missed
                        timer -= 10.0f;
                    }

                    ball_velocity.y *= ball_collision.y;
                    ball_velocity.x *= ball_collision.x;

                        ball_collision = CheckCollisionBoxPro({ball_pos.x, ball_pos.y, ball_radius, ball_radius}, 
                            {left_paddle_pos.x, left_paddle_pos.y, left_paddle_size.x, left_paddle_size.y}, ball_velocity);

                        ball_collision = Vector2Multiply(ball_collision, CheckCollisionBoxPro({ball_pos.x, ball_pos.y, ball_radius, ball_radius}, 
                            {right_paddle_pos.x, right_paddle_pos.y, right_paddle_size.x, right_paddle_size.y}, ball_velocity));

                    if(ball_collision.y <= 0.0f){ //Timer Extended!
                        timer += 3.0f; 
                    }

                    ball_velocity.y *= ball_collision.y;
                    ball_velocity.x *= ball_collision.x;

                    ball_pos.x = std::fmax(0, std::fmin(static_cast<float>(screenWidth) - ball_radius, ball_pos.x));
                    ball_pos.y = std::fmax(0, std::fmin(static_cast<float>(screenHeight) - ball_radius, ball_pos.y));
                }
                #pragma endregion

            // ===========================================================  
            // Main Window Iteration Logic
            // ===========================================================
                #pragma region Main Window Iteration Logic
                    SetActiveWindowContext(window_main);

                    if(game_state == 1){
                        main_window_pos.x = main_window_pos.x + (main_window_velocity.x * logicDelta);
                        main_window_pos.y = main_window_pos.y + (main_window_velocity.y * logicDelta);

                        //My hatred for this code is immeasurable
                        //And my day is ruined

                        Vector2 main_window_collision = CheckCollisionBoundary({main_window_pos.x, main_window_pos.y, main_window_dim.x, main_window_dim.y}, 
                            {0, 0, screenWidth, screenHeight}, main_window_velocity);

                        main_window_velocity.x *= main_window_collision.x;
                        main_window_velocity.y *= main_window_collision.y;

                        main_window_pos.x = std::fmax(0, std::fmin(screenWidth - main_window_dim.x, main_window_pos.x));
                        main_window_pos.y = std::fmax(0, std::fmin(screenHeight - main_window_dim.y, main_window_pos.y));
                    } else if (game_state == 1 || game_state == 3){
                        do{
                            DO_WINDOW_GRAVITY(main_window_pos, main_window_velocity, main_window_dim);

                            main_window_pos = window_pos;
                            main_window_dim = window_dim;
                            main_window_velocity = window_velocity;
                        } while(0);
                    }
                    SetWindowPosition((int)main_window_pos.x, (int)main_window_pos.y);  
                    SetWindowSize(main_window_dim.x, main_window_dim.y);
                #pragma endregion

            // ===========================================================
            // Scoreboard Window Iteration Logic
            // ===========================================================
                #pragma region Scoreboard Window Iteration Logic

                SetActiveWindowContext(scoreboard_window);

                Vector2 scoreboard_window_collision = CheckCollisionBoundary({scoreboard_window_coords.x, scoreboard_window_coords.y, scoreboard_window_dim.x, scoreboard_window_dim.y}, 
                {0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, scoreboard_window_velocity);

                Vector2 mouse_pos = GetMousePosition();
                Vector2 mouse_delta = GetMouseDelta();
                float mouse_delta_absolute = Vector2Length(mouse_delta);
                
                float default_multiplier = 1.3f;
                if(game_state == 0 || game_state == 3){
                    default_multiplier = 3.0f;
                }

                Vector2 old_scoreboard_window_dim = scoreboard_window_dim; //For shrinking to the center as opposed to the corner.
                Vector2 old_scoreboard_window_velocity = {scoreboard_window_velocity.x, scoreboard_window_velocity.y};

                if(IsKeyDown(KEY_SPACE)){
                    scoreboard_window_dim.x = lerp(scoreboard_window_dim.x, scoreboard_window_dim_default.x * default_multiplier, 0.1f);
                    scoreboard_window_dim.y = lerp(scoreboard_window_dim.y, scoreboard_window_dim_default.y * default_multiplier, 0.1f);

                    scoreboard_window_coords.x -= (scoreboard_window_dim.x - old_scoreboard_window_dim.x)/2; //Shirnk compensation
                    scoreboard_window_coords.y -= (scoreboard_window_dim.y - old_scoreboard_window_dim.y)/2;


                    if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)){
                        scoreboard_window_coords.x = lerp(scoreboard_window_coords.x, scoreboard_window_coords.x + mouse_pos.x - scoreboard_window_dim.x/2, 0.5f);
                        scoreboard_window_coords.y = lerp(scoreboard_window_coords.y, scoreboard_window_coords.y + mouse_pos.y - scoreboard_window_dim.y/2, 0.5f);
            
                    }

                    scoreboard_window_coords.x = std::fmax(0, std::fmin(static_cast<float>(screenWidth) - scoreboard_window_dim.x, scoreboard_window_coords.x));
                    scoreboard_window_coords.y = std::fmax(0, std::fmin(static_cast<float>(screenHeight) - scoreboard_window_dim.y, scoreboard_window_coords.y));

                } else {
                    scoreboard_window_coords.x = scoreboard_window_coords.x + (scoreboard_window_velocity.x * logicDelta);
                    scoreboard_window_coords.y = scoreboard_window_coords.y + (scoreboard_window_velocity.y * logicDelta); 

                    scoreboard_window_velocity.x *= scoreboard_window_collision.x;
                    scoreboard_window_velocity.y *= scoreboard_window_collision.y; 
                
                    scoreboard_window_velocity.y += gravity * logicDelta;
                    scoreboard_window_velocity = Vector2Multiply(scoreboard_window_velocity, {pow(drag, logicDelta), pow(drag, logicDelta)});

                    if(Vector2Length(scoreboard_window_velocity) < 5.0f && 
                        (old_scoreboard_window_velocity.x/fabs(old_scoreboard_window_velocity.x) != scoreboard_window_velocity.x/fabs(scoreboard_window_velocity.x))){
                        scoreboard_window_velocity.x = 0.0f;
                        scoreboard_window_velocity.y = 0.0f;
                    }

                    if(scoreboard_window_velocity.x == 0.0f && scoreboard_window_velocity.y == 0.0f){
                        if(gravity > 0){
                            scoreboard_window_coords.y = lerp(scoreboard_window_coords.y, screenHeight - scoreboard_window_dim.y, 0.1f);
                        } else {
                            scoreboard_window_coords.y = lerp(scoreboard_window_coords.y, 0, 0.1f);
                        }
                    }

                    scoreboard_window_dim.x = lerp(scoreboard_window_dim.x, scoreboard_window_dim_default.x, 0.1f);
                    scoreboard_window_dim.y = lerp(scoreboard_window_dim.y, scoreboard_window_dim_default.y, 0.1f);

                    scoreboard_window_coords.x -= (scoreboard_window_dim.x - old_scoreboard_window_dim.x)/2; //Shirnk compensation
                    scoreboard_window_coords.y -= (scoreboard_window_dim.y - old_scoreboard_window_dim.y)/2;

                    scoreboard_window_coords.x = std::fmax(0, std::fmin(static_cast<float>(screenWidth) - scoreboard_window_dim.x, scoreboard_window_coords.x));
                    scoreboard_window_coords.y = std::fmax(0, std::fmin(static_cast<float>(screenHeight) - scoreboard_window_dim.y, scoreboard_window_coords.y));
                }

                SetWindowPosition((int)scoreboard_window_coords.x, (int)scoreboard_window_coords.y);
                SetWindowSize(scoreboard_window_dim.x, scoreboard_window_dim.y);

                #pragma endregion

            // ===========================================================  
            // Paddle Windows Iteration Logic
            // ===========================================================
                #pragma region Paddle Windows Iteration Logic

                SetActiveWindowContext(left_paddle_window);

                if(game_state == 1){
                    left_paddle_window_pos.y = lerp(left_paddle_window_pos.y, (left_paddle_pos.y+left_paddle_size.y/2)-((float)DEFAULT_WINDOW_WIDTH/2.0f), 0.1f);
                    left_paddle_window_pos.y = std::fmax(0, std::fmin(screenHeight - DEFAULT_WINDOW_WIDTH, left_paddle_window_pos.y));
                } else if(game_state == 0 || game_state == 3){
                    do{
                        DO_WINDOW_GRAVITY(left_paddle_window_pos, left_paddle_window_velocity, left_paddle_window_dim);

                        left_paddle_window_pos = window_pos;
                        left_paddle_window_velocity = window_velocity;
                        left_paddle_window_dim = window_dim;
                    } while(0);
                }
                
                SetWindowPosition((int)left_paddle_window_pos.x, (int)left_paddle_window_pos.y);

                SetActiveWindowContext(right_paddle_window);

                if(game_state == 1){
                    right_paddle_window_pos.y = lerp(right_paddle_window_pos.y, (right_paddle_pos.y+right_paddle_size.y/2)-((float)DEFAULT_WINDOW_WIDTH/2.0f), 0.1f);
                    right_paddle_window_pos.y = std::fmax(0, std::fmin(screenHeight - DEFAULT_WINDOW_WIDTH, right_paddle_window_pos.y));
                } else if(game_state == 0 || game_state == 3){
                    do{
                        DO_WINDOW_GRAVITY(right_paddle_window_pos, right_paddle_window_velocity, right_paddle_window_dim);

                        right_paddle_window_pos = window_pos;
                        right_paddle_window_velocity = window_velocity;
                        right_paddle_window_dim = window_dim;
                    } while(0);
                }
                

                SetWindowPosition((int)right_paddle_window_pos.x, (int)right_paddle_window_pos.y);

                #pragma endregion

            #pragma endregion
        
        // ===========================================================
        // Draw Block
        // ==========================================================
           #pragma region Draw Block 
            // ===========================================================
            // Draw Logics
            // ===========================================================
            #pragma region Draw Logics

                SetActiveWindowContext(window_main);

                UpdateCamera(&camera, CAMERA_PERSPECTIVE);
                
                Matrix view_matrix = MatrixLookAt(camera.position, camera.target, camera.up);
                float camera_distance = Vector3Distance(camera.position, camera.target);
                int diameter_width = 2 * tan(camera.fovy * DEG2RAD * 0.5) * camera_distance;

                // SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "cameraPos"), &camera.position, SHADER_UNIFORM_VEC3);
                // SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenWidth"), &screenWidth, SHADER_UNIFORM_INT);
                // SetShaderValue(raymarching_shader, GetShaderLocation(raymarching_shader, "screenHeight"), &screenHeight, SHADER_UNIFORM_INT);  
                // SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "view"), view_matrix);
                // SetShaderValueMatrix(raymarching_shader, GetShaderLocation(raymarching_shader, "projection"), MatrixPerspective(camera.fovy, (double)GetScreenWidth()/(double)GetScreenHeight(), 0.1, 1000.0));

            #pragma endregion

            // ===========================================================
            // Render Block 
            // ===========================================================
            #pragma region Render Block

                // ===========================================================
                // Window MAIN Draw
                // ===========================================================
                    #pragma region Window MAIN Draw

                        SetActiveWindowContext(window_main);

                        do{ //Macros and scope operators are awesome
                            GET_WINDOW_INFO;

                            GET_NORMALIZED_WINDOW_INFO;

                            UPDATE_CAMERA;
                        } while (0);
                        
                        BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DRAW_3D_SCENE;

                            GAME_DRAW_LOGIC(window_main, window_main);
                        EndDrawing();
                    
                     RESET_CAMERA;
                    #pragma endregion

                    // ===========================================================
                    // Left Paddle Window Draw
                    // ===========================================================
                    #pragma region  Left Paddle Window Draw
                        SetActiveWindowContext(left_paddle_window);

                        do{ //WHO THE HELL NNEEDS OBJECTS WHEN YOU HAVE MACROOSS AND SCOPE OPERATORS SBABYYYYYYYYYYYYYYYYY (╯°□°）╯︵ ┻━┻
                            GET_WINDOW_INFO;

                            GET_NORMALIZED_WINDOW_INFO;

                            UPDATE_CAMERA;
                        } while (0);
                        
                        BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DRAW_3D_SCENE;
                            GAME_DRAW_LOGIC(left_paddle_window, left_paddle_window);
                        EndDrawing();
                    
                        RESET_CAMERA;

                        #pragma endregion

                    // ===========================================================
                    // Right Paddle Window Draw
                    // ===========================================================
                    #pragma region Right Paddle Window Draw

                    SetActiveWindowContext(right_paddle_window);

                    do{ //WHO THE HELL NNEEDS OBJECTS WHEN YOU HAVE MACROOSS AND SCOPE OPERATORS SBABYYYYYYYYYYYYYYYYY (╯°□°）╯︵ ┻━┻
                        GET_WINDOW_INFO;

                        GET_NORMALIZED_WINDOW_INFO;

                        UPDATE_CAMERA;
                    } while (0);
                    
                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        DRAW_3D_SCENE;
                        GAME_DRAW_LOGIC(right_paddle_window, right_paddle_window);
                    EndDrawing();
                
                    RESET_CAMERA;

                    #pragma endregion

                // ===========================================================
                // Scoreboard Draw
                // ===========================================================
                    #pragma region Scoreboard Draw

                    SetActiveWindowContext(scoreboard_window);
                    
                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        // BeginShaderMode(raymarching_shader);
                            DRAW_3D_SCENE;
                        // EndShaderMode();

                        do{ //Visualize Main Window Area
                            SetActiveWindowContext(window_main);
                            GET_WINDOW_INFO;
                            
                            SetActiveWindowContext(scoreboard_window);
                            DRAW_VIEWPORT_VIEW;
                        } while (0);

                        do{ //Visualize Left Paddle Area
                            SetActiveWindowContext(left_paddle_window);
                            GET_WINDOW_INFO;
                            
                            SetActiveWindowContext(scoreboard_window);
                            DRAW_VIEWPORT_VIEW;
                        } while (0);

                        do{ //Visualize Right Paddle Area
                            SetActiveWindowContext(right_paddle_window);
                            GET_WINDOW_INFO;
                            
                            SetActiveWindowContext(scoreboard_window);
                            DRAW_VIEWPORT_VIEW;
                        } while (0);

                        do{ //Visualize Scoreboard Area
                            SetActiveWindowContext(scoreboard_window);
                            GET_WINDOW_INFO;
                            
                            SetActiveWindowContext(scoreboard_window);
                            DRAW_VIEWPORT_VIEW;
                        } while (0);

                        SetActiveWindowContext(scoreboard_window);
                            GET_WINDOW_INFO;
                        
                        if(game_state == 0){
                            if(IsKeyDown(KEY_SPACE)){
                                DrawText("Hold [SPACE] to Expand Scoreboard", 10, window_dim.y-80, 20, BLACK);
                                DrawText("Hold [SPACE] and [Middle Mouse Button] to Move Scoreboard", 10, window_dim.y-50, 20, BLACK);

                                if(IsKeyDown(KEY_T)){
                                    DrawText("Leaderboard", 10, 200, 50, BLACK);
                                    DrawText(std::format("1. {} - {:.2f}", rank_1_name, rank_1_time).c_str(), 10, 250, 20, BLACK);
                                    DrawText(std::format("2. {} - {:.2f}", rank_2_name, rank_2_time).c_str(), 10, 270, 20, BLACK);
                                    DrawText(std::format("3. {} - {:.2f}", rank_3_name, rank_3_time).c_str(), 10, 290, 20, BLACK);
                                    DrawText(std::format("4. {} - {:.2f}", rank_4_name, rank_4_time).c_str(), 10, 310, 20, BLACK);
                                    DrawText(std::format("5. {} - {:.2f}", rank_5_name, rank_5_time).c_str(), 10, 330, 20, BLACK);
                                }
                                
                                DrawText("Player 1: Press [W] and [S] to Move Left Paddle", 10, 10, 20, BLACK);
                                DrawText("Player 2: Press [UP] and [DOWN] to Move Right Paddle", 10, 30, 20, BLACK);
                                DrawText("Yell to Accelerate the Game(check your terminal for visualizer)", 10, 50, 20, BLACK);
                                DrawText("Press [T] to Show Leaderboard", 10, 70, 20, BLACK);
                                DrawText("Press [ESC] to Quit", 10, 90, 20, BLACK); 

                                DrawText("Press [ENTER] to Start Game", 10, 110, 20, BLACK);
                            } else {
                                DrawText("Hold [SPACE] to Hold Scoreboard", 10, 50, 20, BLACK);
                                DrawText("Hold [SPACE] and [Middle Mouse Button] \n to Move Scoreboard", 10, 10, 20, BLACK);
                            }
                        }

                        if(game_state == 1){
                            DrawText("Hold [SPACE] to Hold Scoreboard", 10, window_dim.y-80, 20, BLACK);
                            DrawText("Hold [SPACE] and [Middle Mouse Button] \nto Move Scoreboard", 10, window_dim.y-50, 20, BLACK);

                            DrawText(std::format("Time Left: {:.2f}", timer).c_str(), 10, 30, 20, BLACK);
                            DrawText(std::format("Total Time Survived: {:.2f}", total_time).c_str(), 10, 50, 20, BLACK);
                        }
                    
                        if(game_state == 3){
                            DrawText("GAME OVER!", 10, 70, 20, BLACK);
                            if(total_time >= rank_5_time){
                                DrawText("You made it to the leaderboard!", 10, 90, 20, BLACK);
                                DrawText("Enter your name: ", 10, 110, 20, BLACK);
                                DrawText("Press [SHIFT] to Submit", 10, 130, 20, BLACK);
                                static char player_name[20] = "\0";
                                static int letter_count = 0;

                                int key = GetCharPressed();

                                while (key > 0) {
                                    if ((key >= 32) && (key <= 125) && (letter_count < 20)) {
                                        player_name[letter_count] = (char)key;
                                        player_name[letter_count + 1] = '\0';
                                        letter_count++;
                                    }

                                    key = GetCharPressed();
                                }

                                if (IsKeyPressed(KEY_BACKSPACE)) {
                                    letter_count--;
                                    if (letter_count < 0) letter_count = 0;
                                    player_name[letter_count] = '\0';
                                }

                                DrawText(player_name, 200, 110, 20, BLACK);

                                if ((IsKeyPressed(KEY_RIGHT_SHIFT) || IsKeyPressed(KEY_LEFT_SHIFT)) && letter_count > 0) {
                                    std::ofstream leaderboard_out("../leaderboard.txt");

                                    if (total_time >= rank_1_time) {
                                        rank_5 = rank_4;
                                        rank_4 = rank_3;
                                        rank_3 = rank_2;
                                        rank_2 = rank_1;
                                        rank_1 = std::format("{} {:.2f}", player_name, total_time);
                                    } else if (total_time >= rank_2_time) {
                                        rank_5 = rank_4;
                                        rank_4 = rank_3;
                                        rank_3 = rank_2;
                                        rank_2 = std::format("{} {:.2f}", player_name, total_time);
                                    } else if (total_time >= rank_3_time) {
                                        rank_5 = rank_4;
                                        rank_4 = rank_3;
                                        rank_3 = std::format("{} {:.2f}", player_name, total_time);
                                    } else if (total_time >= rank_4_time) {
                                        rank_5 = rank_4;
                                        rank_4 = std::format("{} {:.2f}", player_name, total_time);
                                    } else {
                                        rank_5 = std::format("{} {:.2f}", player_name, total_time);
                                    }

                                    leaderboard_out << rank_1 << std::endl;
                                    leaderboard_out << rank_2 << std::endl;
                                    leaderboard_out << rank_3 << std::endl;
                                    leaderboard_out << rank_4 << std::endl;
                                    leaderboard_out << rank_5 << std::endl;

                                    leaderboard_out.close();
                                    game_state = 0;
                                    timer = 30.0f;
                                    total_time = 0.0f;
                                    letter_count = 0;
                                    player_name[0] = '\0';
                                }
                            } else {
                                DrawText("You didn't make it to the leaderboard :P", 10, 90, 20, BLACK);
                                DrawText("Press [ENTER] to Restart", 10, 110, 20, BLACK);
                            }
                        }

                    EndDrawing();

                    #pragma endregion
            #pragma endregion
    }

    // ===========================================================
    // Clean-up
    // ===========================================================
        
    #pragma region Clean-up
        
    //Clean-up
    ma_device_uninit(&device);

    UnloadMusicStream(game_state_1_music);
    UnloadTexture(cow_texture);
    UnloadModel(cow);
    UnloadModel(cube);

    // UnloadShader(raymarching_shader);

    SetActiveWindowContext(window_main);
    CloseWindow();

    SetActiveWindowContext(scoreboard_window);
    CloseWindow();

    SetActiveWindowContext(left_paddle_window);
    CloseWindow();

    SetActiveWindowContext(right_paddle_window);
    CloseWindow();

    #pragma endregion   


    return 0;
}