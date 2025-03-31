/**
 * @file input_behavior_a.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once


#include <iostream>
#include <vector>
#include <memory>

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

#include "BufferedRaylib.hpp"
#include "kinematics_component.hpp"

namespace CO{
    
    class AS7InputControllerBehavior : public Behavior {
        static raylib::BufferedInput input; // Input of the entity
        std::shared_ptr<CO::Entity> leftPaddle = nullptr; // Left paddle entity
        std::shared_ptr<CO::Entity> rightPaddle = nullptr; // Right paddle entity
        std::shared_ptr<CO::Entity> scoreBoard = nullptr; // Scoreboard entity
        std::vector<int> window_ids; // Vector of all relevant window IDs
        
        public: 
            //=================================================================
            /// CONSTRUCTORS/COPY LOGIC
            //=================================================================
            AS7InputControllerBehavior() = delete; // Delete the default constructor to prevent instantiation without parameters
            AS7InputControllerBehavior(Entity* entity, std::shared_ptr<CO::Entity> leftPaddle, std::shared_ptr<CO::Entity> rightPaddle, std::shared_ptr<CO::Entity> scoreBoard, std::vector<int> window_ids) : Behavior(entity), leftPaddle(leftPaddle), rightPaddle(rightPaddle), scoreBoard(scoreBoard), window_ids(window_ids) { // Constructor taking in a target entity
                checkRequirements(); // Check if the requirements are met
                configure(); 
            } // Constructor taking in a target entity
            AS7InputControllerBehavior(const AS7InputControllerBehavior& other) = default;
            AS7InputControllerBehavior& operator=(const AS7InputControllerBehavior& other) = default;

            //=================================================================
            // ITRATION LOGIC
            //=================================================================
        
            void Tick(const float& deltaTime) override {}

            void Update() override {
                for(auto& window_id : window_ids) {
                    SetActiveWindowContext(window_id); // Set the active window context to the specific window
                    input.PollEvents();
                }
            }

            //=================================================================
            /// CONFIGURATION LOGIC
            //=================================================================
            void configure() {
                input.actions["left_paddle"] = raylib::Action::button_axis(
                    {raylib::Button::key(KEY_W)}, 
                    {raylib::Button::key(KEY_S)}
                ).move();

                input["left_paddle"].AddCallback([this](float state, float change){
                    if(state == change){
                        leftPaddle->getComponent<CO::KinematicsComponent>().value().get().setVelocity({0, -10*state, 0}); // Set the velocity of the left paddle
                    } else {
                        leftPaddle->getComponent<CO::KinematicsComponent>().value().get().setVelocity({0, 0, 0}); // Set the velocity of the left paddle to zero when the key is released
                    }
                });

                input.actions["right_paddle"] = raylib::Action::button_axis(
                    {raylib::Button::key(KEY_UP)}, 
                    {raylib::Button::key(KEY_DOWN)}
                ).move();

                input["right_paddle"].AddCallback([this](float state, float change){
                    if(state == change){
                        rightPaddle->getComponent<CO::KinematicsComponent>().value().get().setVelocity({0, -10*state, 0}); // Set the velocity of the right paddle
                    } else {
                        rightPaddle->getComponent<CO::KinematicsComponent>().value().get().setVelocity({0, 0, 0}); // Set the velocity of the right paddle to zero when the key is released
                    }
                });

            }
            
            std::unique_ptr<Component> clone() const override { return std::make_unique<AS7InputControllerBehavior>(*this); }
            
            bool checkRequirements() override { 
                bool leftCompatible = leftPaddle->hasComponent<CO::KinematicsComponent>();
                bool rightCompatible = rightPaddle->hasComponent<CO::KinematicsComponent>();
                bool scoreCompatible = scoreBoard->hasComponent<CO::KinematicsComponent>();
                if (!leftCompatible || !rightCompatible || !scoreCompatible) throw std::runtime_error("AS7InputControllerBehavior requires a KinematicsComponent on the left paddle, right paddle, and scoreboard"); // Check if the target entity has a box collision component and kinematics component 
                return true;
             }
    };
}

raylib::BufferedInput CO::AS7InputControllerBehavior::input = raylib::BufferedInput(); // Initialize the input
