/**
 * @file as7.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <chrono>

 // Raylib include
#include "raylib.h"	

// DEFAULT RENDER SETTINGs
#define DEFAULT_SCALE 10
#define DRAW_FPS 60
#define LOGIC_FPS 60

#define PLANE_WIDTH 1000
#define PLANE_LENGTH 1000

// DEFAULT INTERFACE SETTINGs
#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING

#define DEFAULT_WINDOW_WIDTH 50
#define DEFAULT_WINDOW_HEIGHT 50

#define SCOREBOARD_WIDTH 600
#define SCOREBOARD_HEIGHT 600

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 300

#define DEFAULT_TITLE "CS381 - Assignment 7"

//#include "skybox.hpp"

// Base Components
#include "component.hpp"
#include "entity.hpp"
#include "behavior.hpp"

// Derivative Components
#include "camera_component.hpp"    
#include "render_component.hpp"     
#include "transform_component.hpp"  
#include "kinematics_component.hpp" 
//#include "oriented_kinematics_component.hpp"  
#include "collision_component.hpp"
#include "box_collision_component.hpp"
#include "image_window_render_component.hpp"

// Derivative Input Behaviors
//#include "input_behavior_a.hpp"

// Derivative Window-Related Components
#include "window_component.hpp"
#include "window_render_component.hpp"
#include "image_window_render_component.hpp"

// Derivative Behaviors
//#include "transform_bind_behavior.hpp"
//#include "oriented_kinematics_behavior.hpp"
#include "follow_camera_behavior.hpp"
#include "window_bind_behavior.hpp"
#include "box_boundary_behavior.hpp"
#include "box_reflection_behavior.hpp"
#include "box_collision_bind_behavior.hpp"
#include "as7_input_controller_behavior.hpp"
#include "kinematics_controller_behavior.hpp"
#include "gravity_behavior.hpp" 


// Entity Skeletons
//#include "oriented_kinematics_entity.hpp"

std::shared_ptr<CO::Entity> make_kinematic_window_entity(std::string title, Vector2 pos, Vector2 dim, Vector2 boundaryDim, 
    std::shared_ptr<std::vector<std::shared_ptr<CO::Entity>>> collisionEntities = nullptr, bool isMainWindow = false) {
    auto entity = std::make_shared<CO::Entity>();
    entity->addComponent<CO::WindowComponent, Vector2, Vector2, std::string>(pos, dim, title);
    entity->addComponent<CO::KinematicsComponent, Vector3>({pos.x, pos.y, 0});
    entity->addComponent<CO::BoxCollisionComponent, Rectangle>(Rectangle{pos.x, pos.y, dim.x, dim.y});

    entity->addComponent<CO::TransformComponent, Matrix>(MatrixIdentity());
    entity->getComponent<CO::TransformComponent>()->get().setPosition({pos.x, pos.y, 0});
    entity->getComponent<CO::TransformComponent>()->get().setScale({dim.x, dim.y, 0});

    if(isMainWindow){
        entity->addComponent<CO::BoxReflectionBehavior, CO::Entity*, std::shared_ptr<std::vector<std::shared_ptr<CO::Entity>>>, Rectangle>(entity.get(), collisionEntities, {0, 0, boundaryDim.x, boundaryDim.y});
    }
    entity->addComponent<CO::GravityBehavior, CO::Entity*>(entity.get());
    entity->addComponent<CO::KinematicsControllerBehavior, CO::Entity*>(entity.get());
    entity->addComponent<CO::BoxBoundaryBehavior, CO::Entity*, Rectangle>(entity.get(), {0, 0, boundaryDim.x, boundaryDim.y});
    entity->addComponent<CO::BoxCollisionBindBehavior, CO::Entity*>(entity.get());
    entity->addComponent<CO::WindowBindBehavior, CO::Entity*>(entity.get());

    return entity;
}

// ===========================================================
// Main Function
// ===========================================================

int main(){
    // ===========================================================
    // Initialization
    // ===========================================================
        InitWindow(0, 0, "quick fix"); // Initialize the window with a default size
        Vector2 screenDim = {static_cast<float>(GetMonitorWidth(0)), static_cast<float>(GetMonitorHeight(0))}; // Get the screen dimensions
        CloseWindow(); // Close the default window

        enum {MENU, PLAYING, GAME_OVER}; // Define the game states
        int game_state = MENU; // Initialize the game state to MENU

        bool closeWindow = false;
        // ===========================================================
        // Important Variables
        // ===========================================================
        double previous_time = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
        double current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
            
        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================

        // ===========================================================
        // Audio Initialization
        // ===========================================================

        InitAudioDevice(); // Initialize the audio device
        Music music = LoadMusicStream("../../custom_assets/as4/cats_on_mars.mp3"); // Load the music stream
        PlayMusicStream(music); // Play the music stream
        
        /// ^^^ so opengl doesn't freak the hell out

        // ===========================================================
        // Component + Entity Initialization
        // ===========================================================
        std::vector<std::shared_ptr<CO::Entity>> entities;
        auto system_entities = std::make_shared<std::vector<std::shared_ptr<CO::Entity>>>();
        auto collision_layer_1 = std::make_shared<std::vector<std::shared_ptr<CO::Entity>>>();

        entities.push_back(make_kinematic_window_entity("Scoreboard", {(screenDim.x - SCOREBOARD_WIDTH)/2, (screenDim.y - SCOREBOARD_HEIGHT)/2}, {SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT}, {screenDim.x, screenDim.y}));
        
        SetActiveWindowContext(entities[0]->getComponent<CO::WindowComponent>()->get().getWindowId()); // Set the active window context to the scoreboard window
        std::shared_ptr<Texture2D> firelink_texture = std::make_shared<Texture2D>(LoadTexture("../../custom_assets/firelink.png")); // Load the texture
        firelink_texture->width = SCOREBOARD_WIDTH; // Set the width of the texture
        firelink_texture->height = SCOREBOARD_HEIGHT; // Set the height of the texture
        // ^^^ goofy open gl context stuff
        
        entities.push_back(make_kinematic_window_entity("Left Paddle", {PADDLE_WIDTH, PADDLE_WIDTH}, {PADDLE_WIDTH, PADDLE_HEIGHT}, {screenDim.x, screenDim.y},
            collision_layer_1));
        entities.push_back(make_kinematic_window_entity("Right Paddle", {screenDim.x - (PADDLE_WIDTH + PADDLE_WIDTH), PADDLE_WIDTH}, {PADDLE_WIDTH, PADDLE_HEIGHT}, {screenDim.x, screenDim.y},
            collision_layer_1));
        entities.push_back(make_kinematic_window_entity("Ball", {screenDim.x/2 - DEFAULT_WINDOW_WIDTH/2, screenDim.y/2 - DEFAULT_WINDOW_HEIGHT/2}, {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}, {screenDim.x, screenDim.y}, 
            collision_layer_1, true));
        
        std::vector<int> window_ids;

        for(auto& entity : entities){
            if(entity->hasComponent<CO::WindowComponent>()){
                window_ids.push_back(entity->getComponent<CO::WindowComponent>()->get().getWindowId()); // Get the ID of the window
            }
        }
        entities.push_back(std::make_shared<CO::Entity>()); 
        entities[4]->addComponent<CO::AS7InputControllerBehavior, CO::Entity*, std::shared_ptr<CO::Entity>, std::shared_ptr<CO::Entity>, std::shared_ptr<CO::Entity>, std::vector<int>>(
            entities[4].get(), entities[1], entities[2], entities[0], window_ids); // Add the input controller behavior to the entity

        entities[3]->getComponent<CO::KinematicsComponent>()->get().setVelocity({5, 5, 0});
        // Initial Transformations
        collision_layer_1->push_back(entities[1]);
        collision_layer_1->push_back(entities[2]);
        collision_layer_1->push_back(entities[3]);
        system_entities->push_back(entities[4]);

        for(auto& entity : entities){
            if(entity->hasComponent<CO::GravityBehavior>()){
                entity->getComponent<CO::GravityBehavior>()->get().setGravity(2.81f); // Set the gravity value for the entity
                entity->getComponent<CO::GravityBehavior>()->get().setEnabled(false); // Disable the gravity behavior for the entity
            }
        }

        entities[0]->getComponent<CO::GravityBehavior>()->get().setGravity(2.81f); // Set the gravity value for the entity
        entities[0]->getComponent<CO::GravityBehavior>()->get().setEnabled(true);
        entities[0]->addComponent<CO::ImageWindowRenderComponent, int, std::shared_ptr<Texture2D>, Vector2, Vector2>(
            entities[0]->getComponent<CO::WindowComponent>()->get().getWindowId(), firelink_texture, {0, 0}, {SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT}
        );

    while (!closeWindow){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================
            UpdateMusicStream(music); // Update the music stream
        // ===========================================================
        // Delta Logic Block
        // ===========================================================
            current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
            
            double logicFrameTime = current_time - previous_time;
            logicFrameTime = (logicFrameTime < 0.0f) ? 0.0f : logicFrameTime; // Clamp the logic frame time to a minimum of 0.0f
            // Some really goofy clock skew issues go on on my pc, so this is a goofy workaround
            double logicDelta = logicFrameTime * LOGIC_FPS;

            previous_time = current_time;

            //============================================================
            // Entity Tick + Update Logic
            //============================================================

            for(auto& entity : entities){
                entity->Tick(logicDelta);
                entity->Update();
            }

        // ===========================================================
        // Draw Block
        // ==========================================================

            // ===========================================================
            // Render Block 
            // ===========================================================
            for(auto& entity : entities){
                if(entity->hasComponent<CO::WindowComponent>()){
                    SetActiveWindowContext(entity->getComponent<CO::WindowComponent>()->get().getWindowId()); // Set the active window context to the specific window
                    ClearBackground(BLACK); // Clear the background of the window
                    closeWindow = closeWindow || WindowShouldClose(); // Check if the window should close

                    BeginDrawing();

                    for(auto& entity : entities){
                        if(entity->hasComponent<CO::CameraComponent>()){
                            entity->getComponent<CO::CameraComponent>()->get().BeginCameraMode();
                        }
                    }


                    // ===========================================================
                    // Background Draw
                    // ===========================================================



                    // ===========================================================
                    // Midground Draw
                    // ===========================================================

                    for(auto& entity : entities){
                        if(entity->hasComponent<CO::RenderComponent>()){
                            entity->getComponent<CO::RenderComponent>()->get().Render();
                        }
                    }

                    // ===========================================================
                    // Foreground Draw
                    // ===========================================================

                    for(auto& entity : entities){
                        if(entity->hasComponent<CO::ImageWindowRenderComponent>()){
                            entity->getComponent<CO::ImageWindowRenderComponent>()->get().Render();
                        }
                    }
                    
                    // Coordinate axes draw
                    // DrawLine3D({0, 0, 0}, {30, 0, 0}, RED);
                    // DrawLine3D({0, 0, 0}, {0, 30, 0}, GREEN);
                    // DrawLine3D({0, 0, 0}, {0, 0, 30}, BLUE);

                    for(auto& entity : entities){
                        if(entity->hasComponent<CO::CameraComponent>()){
                            entity->getComponent<CO::CameraComponent>()->get().EndCameraMode();
                        }
                    }

                    

                    EndDrawing();
                }
            }
            

    }
    UnloadTexture(*firelink_texture.get()); // Unload the texture
    for(auto& entity : entities){
        if(entity->hasComponent<CO::WindowComponent>()){
            SetActiveWindowContext(entity->getComponent<CO::WindowComponent>()->get().getWindowId()); // Set the active window context to the specific window
            CloseWindow(); // Close the window
        }
    }
    return 0;
}