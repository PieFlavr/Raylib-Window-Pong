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

#define DEFAULT_WINDOW_WIDTH 400
#define DEFAULT_WINDOW_HEIGHT 400

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

// Derivative Input Behaviors
//#include "input_behavior_a.hpp"

// Derivative Window-Related Components
#include "window_component.hpp"
#include "window_render_component.hpp"

// Derivative Behaviors
//#include "transform_bind_behavior.hpp"
//#include "oriented_kinematics_behavior.hpp"
#include "follow_camera_behavior.hpp"
#include "kinematics_controller_behavior.hpp"
#include "window_bind_behavior.hpp"
#include "box_boundary_behavior.hpp"
#include "box_reflection_behavior.hpp"
#include "box_collision_bind_behavior.hpp"


// Entity Skeletons
//#include "oriented_kinematics_entity.hpp"

std::shared_ptr<CO::Entity> make_kinematic_window_entity(std::string title, Vector2 pos, Vector2 dim, Vector2 boundaryDim, std::shared_ptr<std::vector<std::shared_ptr<CO::Entity>>> collisionEntities = nullptr) {
    auto entity = std::make_shared<CO::Entity>();
    entity->addComponent<CO::WindowComponent, Vector2, Vector2, std::string>(pos, dim, title);
    entity->addComponent<CO::KinematicsComponent, Vector3>({pos.x, pos.y, 0});
    entity->addComponent<CO::BoxCollisionComponent, Rectangle>(Rectangle{pos.x, pos.y, dim.x, dim.y});

    entity->addComponent<CO::TransformComponent, Matrix>(MatrixIdentity());
    entity->getComponent<CO::TransformComponent>()->get().setPosition({pos.x, pos.y, 0});
    entity->getComponent<CO::TransformComponent>()->get().setScale({dim.x, dim.y, 0});

    entity->addComponent<CO::BoxBoundaryBehavior, CO::Entity*, Rectangle>(entity.get(), {0, 0, boundaryDim.x, boundaryDim.y});
    entity->addComponent<CO::BoxReflectionBehavior, CO::Entity*, std::shared_ptr<std::vector<std::shared_ptr<CO::Entity>>>>(entity.get(), collisionEntities);
    entity->addComponent<CO::KinematicsControllerBehavior, CO::Entity*>(entity.get());
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

        // ===========================================================
        // Component + Entity Initialization
        // ===========================================================
        std::vector<std::shared_ptr<CO::Entity>> entities;
        std::vector<std::shared_ptr<CO::Entity>> system_entities;
        std::vector<std::shared_ptr<CO::Entity>> controllable_entities;
        std::vector<std::shared_ptr<CO::Entity>> collision_layer_1;
        std::vector<std::shared_ptr<CO::Entity>> collision_layer_2;

        entities.push_back(make_kinematic_window_entity("Main Window", {100, 100}, {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}, {screenDim.x, screenDim.y}, 
            std::make_shared<std::vector<std::shared_ptr<CO::Entity>>>(collision_layer_1)));
        entities.push_back(make_kinematic_window_entity("Left Paddle", {50, 10}, {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}, {screenDim.x, screenDim.y},
            std::make_shared<std::vector<std::shared_ptr<CO::Entity>>>(collision_layer_1)));
        entities.push_back(make_kinematic_window_entity("Right Paddle", {10, 50}, {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}, {screenDim.x, screenDim.y},
            std::make_shared<std::vector<std::shared_ptr<CO::Entity>>>(collision_layer_1)));
        
        entities[0]->getComponent<CO::KinematicsComponent>()->get().setVelocity({5, 5, 0});
        // Initial Transformations
        for(int i = 0; i < entities.size(); i++){

        }

    while (!WindowShouldClose()){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================

        // ===========================================================
        // Delta Logic Block
        // ===========================================================
            current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1e9;
            
            double logicFrameTime = current_time - previous_time;
            double logicDelta = logicFrameTime * LOGIC_FPS;

            // std::cout << "Delta Time: " << logicDelta << std::endl;
            // std::cout << "Delta Frame Time: " << logicFrameTime << std::endl;

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
                    int currentWindowId = entity->getComponent<CO::WindowComponent>()->get().getWindowId();
                    SetActiveWindowContext(currentWindowId); // Set the active window context to the specific window
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
    return 0;
}