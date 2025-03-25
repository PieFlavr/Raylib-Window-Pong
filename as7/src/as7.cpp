/**
 * @file as7.cpp
 * @author Lucas Pinto
 * @brief Kind of a mess, but it works. AS6 with a self-implemented Entity Component System, although not adhering to "purist" ECS design.
 *           There are three main classes, Entity, Component, and Behavior. 
 *           Component is as expected, highly data-driven and primarily informational, but also defines basic interactions. They're modular and self-contained.
 *           Behavior is a big branch class of Component, and applies constraints to existing components. Behaviors are highly dependent on components, and manipulate
 *                  or enforce behaviors on them. Behaviors are somewhat modular, but not as much as Components. They are also more complxes -- essentially local subsystems.
 *           Entity is a class that contains a collection of Components and Behaviors.
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

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

#define DEFAULT_TITLE "CS381 - Assignment 7"

//#include "skybox.hpp"

// Base Components
#include "component.hpp"
#include "entity.hpp"
#include "behavior.hpp"

// Derivative Components
#include "camera_component.hpp"     //in hindsight, a lot of bad design decisions were made
#include "render_component.hpp"     //mostly because for most of these I didn't know how the magic pointering worked
#include "transform_component.hpp"  //and also because I was yet to come up with Behaviors to consolidate component relationships
#include "kinematics_component.hpp" //will probably reconsolidate for AS7 and beyond, especially with kinematics and render seperation
#include "oriented_kinematics_component.hpp"   //but for now, this is what we got (/_ \)
#include "collision_component.hpp"
#include "box_collision_component.hpp"
#include "input_behavior_a.hpp"

// Derivative Behaviors
#include "transform_bind_behavior.hpp"
#include "oriented_kinematics_behavior.hpp"
#include "follow_camera_behavior.hpp"

// Entity Skeletons
#include "oriented_kinematics_entity.hpp"

// Old Transform Macros
//#include "transforms.cpp"

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
        InitWindow(400, 400, (window_title).c_str());

        // ===========================================================
        // Model Loading + Default Transforms
        // ===========================================================

        std::shared_ptr<Model> cow = std::make_shared<Model>(LoadModel("../../custom_assets/as2/cow.glb"));

        std::shared_ptr<Model> sedan = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/sedan.glb"));
        std::shared_ptr<Model> delivery = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/delivery.glb"));
        std::shared_ptr<Model> taxi = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/taxi.glb"));
        std::shared_ptr<Model> garbage_truck = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/garbage-truck.glb"));
        std::shared_ptr<Model> police = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/police.glb"));

        std::shared_ptr<Model> grass = std::make_shared<Model>(LoadModelFromMesh(GenMeshPlane(PLANE_WIDTH, PLANE_LENGTH, 100, 100)));
        Texture2D grassTexture = LoadTexture("../../assets/textures/grass.png");
        grass.get()->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

        //cs381::SkyBox sky("../../assets/textures/skybox.png");

        //std::shared_ptr<Model> wheel = std::make_shared<Model>(LoadModel("../../assets/Kenny Car Kit/wheel-default.glb"));
        std::shared_ptr<Model> rocket = std::make_shared<Model>(LoadModel("../../assets/Kenny Space Kit/rocketA.glb"));
        //cow.transform = MatrixMultiply(MatrixScale(30, 30, 30), MatrixRotateX(90 * DEG2RAD));

        // ===========================================================
        // Audio Initialization
        // ===========================================================

        // ===========================================================
        // Component + Entity Initialization
        // ===========================================================
        std::vector<std::shared_ptr<CO::Entity>> entities;
        

        //In order of unique properties for constructor... Vector3 baseScale, Vector3 baseRotation, float maxSpeed, float maxAcceleration, float turnSpeed
        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(sedan, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 5.0f, 0.05f, 0.025f)));
        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(delivery, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 4.0f, 0.03f, 0.04f)));
        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(taxi, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 6.0f, 0.18f, 0.08f)));
        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(garbage_truck, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 3.0f, 0.01f, 0.005f)));
        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(police, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 8.0f, 0.12f, 0.05f)));

        entities.push_back(std::make_shared<CO::OrientedKinematicEntity>(CO::OrientedKinematicEntity(rocket, {DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE}, {0, 0, 0}, 30.0f, 0.13f, 0.005f)));

        // Initial Transformations
        for(int i = 0; i < 5; i++){
            entities[i]->getComponent<CO::KinematicsComponent>()->get().setPosition({0, 0, (float)(-60 + (i * 30))});
            entities[i]->getComponent<CO::OrientedKinematicsComponent>()->get().setEulerRotation({90*DEG2RAD, 0, 0});
            entities[i]->getComponent<CO::OrientedKinematicsBehavior>()->get().setTargetHeadings(90*DEG2RAD, 0);
            //entities[i]->getComponent<CO::OrientedKinematicsBehavior>()->get().enabled = false;
        }

        //This is kind of dogwater, but atleast for AS7 I know better how to consolidate my components.
        //Plus a lot of lessons with shared and unique pointers I now know about that won't be an issue in the future.
        entities[5]->getComponent<CO::KinematicsComponent>()->get().setPosition({-30.0f, 0, 0});
        entities[5]->getComponent<CO::RenderComponent>()->get().setBaseEulerRotation({90*DEG2RAD, 0, 0});
        entities[5]->getComponent<CO::OrientedKinematicsComponent>()->get().setEulerRotation({0, -90*DEG2RAD, 0});
        entities[5]->getComponent<CO::OrientedKinematicsBehavior>()->get().setTargetHeadings(0, -90*DEG2RAD);

        entities[0]->addComponent<CO::CameraComponent, Camera>(Camera(
            {0, 60, 200}, 
            {0, 0, 0}, 
            {0, 1, 0}, 
            45
        ));
        entities[0]->getComponent<CO::RenderComponent>()->get().arrowsEnabled = true;
        entities[0]->getComponent<CO::RenderComponent>()->get().boundingBoxEnabled = true;

        entities[0]->addComponent<CO::FollowCameraBehavior, CO::Entity*, Vector3, float>(entities[0].get(), Vector3{0, 60, 200}, 0.1f);
        entities[0]->getComponent<CO::FollowCameraBehavior>()->get().enabled = true;

        //This is the worst
        entities[0]->addComponent<CO::InputBehaviorA, CO::Entity*, std::vector<std::shared_ptr<CO::Entity>>*, int>(entities[0].get(), &entities, 0);

    while (!WindowShouldClose()){

        // ===========================================================
        // Non-Delta Logic BLock
        // ===========================================================

        // ===========================================================
        // Delta Logic Block
        // ===========================================================

            double logicFrameTime = GetFrameTime();
            double logicDelta = logicFrameTime * LOGIC_FPS;

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

            BeginDrawing();
            ClearBackground(GRAY);

                for(auto& entity : entities){
                    if(entity->hasComponent<CO::CameraComponent>()){
                        entity->getComponent<CO::CameraComponent>()->get().BeginCameraMode();
                    }
                }


                // ===========================================================
                // Background Draw
                // ===========================================================

                //sky.Draw();
                DrawModel(*grass, {0, 0, 0}, 1.0f, WHITE);

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
    return 0;
}