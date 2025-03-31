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

#include <functional>
#include <iostream>
#include <vector>
#include <memory>

#include "component.hpp"
#include "BufferedRaylib.hpp"

#include "oriented_kinematics_behavior.hpp"
#include "camera_component.hpp"
#include "follow_camera_behavior.hpp"

namespace CO{
    
    class InputBehaviorA : public Behavior {
        static raylib::BufferedInput input; // Input of the entity
        static int selectedEntityIndex; // Index of the selected entity
        std::vector<std::shared_ptr<CO::Entity>> *entities = nullptr; // This is mega-cursed I don't even know anymore
        
        public: 
            //=================================================================
            /// CONSTRUCTORS/COPY LOGIC
            //=================================================================
            InputBehaviorA(Entity* entity, std::vector<std::shared_ptr<CO::Entity>>* entities, int selectedEntityIndex) : Behavior(entity), entities(entities){ selectedEntityIndex = selectedEntityIndex; configure(); } // Constructor taking in a target entity
            InputBehaviorA(const InputBehaviorA& other) = default;
            InputBehaviorA& operator=(const InputBehaviorA& other) = default;

            //=================================================================
            // ITRATION LOGIC
            //=================================================================
        
            void Tick(const float& deltaTime) override {}

            void Update() override {
                input.PollEvents();
            }

            //=================================================================
            /// CONFIGURATION LOGIC
            //=================================================================
            void configure() {
                input.actions["forward"] = raylib::Action::button_axis(
                    {raylib::Button::key(KEY_W)}, 
                    {raylib::Button::key(KEY_S)}
                ).move();

                input["forward"].AddCallback([this](float state, float change){
                    if(state == change){
                        target->getComponent<CO::OrientedKinematicsBehavior>().value().get().incTargetSpeed(0.3 * state);
                    }
                });

                input.actions["rotateXZ"] = raylib::Action::button_axis(
                    {raylib::Button::key(KEY_A)}, 
                    {raylib::Button::key(KEY_D)}
                ).move();

                input["rotateXZ"].AddCallback([this](float state, float change){
                    if(state == change){
                        target->getComponent<CO::OrientedKinematicsBehavior>().value().get().incTargetXZHeading(0.3 * state);
                    }
                });

                input.actions["rotateYZ"] = raylib::Action::button_axis(
                    {raylib::Button::key(KEY_UP)}, 
                    {raylib::Button::key(KEY_DOWN)}
                ).move();

                input["rotateYZ"].AddCallback([this](float state, float change){
                    if(state == change){
                        target->getComponent<CO::OrientedKinematicsBehavior>().value().get().incTargetYZHeading(0.3 * state);
                    }
                });

                input.actions["brake"] = raylib::Action::button(
                    {raylib::Button::key(KEY_SPACE)}
                ).move();

                input["brake"].AddCallback([this](float state, float change){
                    if(state == 0 && change == 1){
                        target->getComponent<CO::OrientedKinematicsBehavior>().value().get().setTargetSpeed(0);
                    }
                });

                input.actions["camera"] = raylib::Action::button(
                    {raylib::Button::key(KEY_TAB)}
                ).move();

                input["camera"].AddCallback([this](float state, float change){

                    // the great leap forward (っ °Д °;)っ
                    if(state == 0 && change == 1){
                        auto cameraBehavior = target->removeComponent<FollowCameraBehavior>();
                        auto camera = target->removeComponent<CameraComponent>();

                        entities->at(selectedEntityIndex)->getComponent<RenderComponent>()->get().arrowsEnabled = false;
                        entities->at(selectedEntityIndex)->getComponent<RenderComponent>()->get().boundingBoxEnabled = false;

                        if (camera) { //bootstrap paradox why is this evenn possible wtf
                            
                            selectedEntityIndex = (selectedEntityIndex + 1) % entities->size();

                            auto self = target->removeComponent<InputBehaviorA>();

                            entities->at(selectedEntityIndex)->pushBackComponent<CameraComponent>(std::move(camera.value()));
                            entities->at(selectedEntityIndex)->pushBackComponent<FollowCameraBehavior>(std::move(cameraBehavior.value()));
                            entities->at(selectedEntityIndex)->pushBackComponent<InputBehaviorA>(std::move(self.value()));

                            entities->at(selectedEntityIndex)->getComponent<RenderComponent>()->get().arrowsEnabled = true;
                            entities->at(selectedEntityIndex)->getComponent<RenderComponent>()->get().boundingBoxEnabled = true;
                        }
                    }
                });
            }
            
            std::unique_ptr<Component> clone() const override { return std::make_unique<InputBehaviorA>(*this); }
            
            bool checkRequirements() override { 
                if(!target->hasComponent<CO::OrientedKinematicsBehavior>()){
                    std::cout << "InputBehaviorA requires an OrientedKinematicsBehavior component" << std::endl;
                    return false;
                }
                return true;
             }
    };
}

raylib::BufferedInput CO::InputBehaviorA::input = raylib::BufferedInput(); // Initialize the input
int CO::InputBehaviorA::selectedEntityIndex = 0; // Initialize the selected entity index