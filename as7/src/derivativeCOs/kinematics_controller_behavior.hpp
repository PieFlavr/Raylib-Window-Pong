/**
 * @file kinematics_bind_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "component.hpp"
#include "entity.hpp"
#include "behavior.hpp"

#include "kinematics_component.hpp"
#include "transform_component.hpp"

namespace CO{
    class KinematicsControllerBehavior : public Behavior {
        public:
            KinematicsControllerBehavior(Entity* target) : Behavior(target) { // Constructor taking in a target entity
                if(!checkRequirements()) throw std::runtime_error("KinematicsControllerBehavior requires a KinematicsComponent and TransformComponent"); // Check if the target entity has a kinematics component and transform component
            }

            void Tick(const float& deltaTime) override {
                if(enabled){ //Syncs the transform component with the target entity's transform component
                    if(target->hasComponent<KinematicsComponent>()) { // Check if the target entity has a kinematics component
                        target->getComponent<CO::TransformComponent>()->get().setPosition(target->getComponent<KinematicsComponent>()->get().getPosition()); // Set the transform component's position to the kinematics component's position
                    } 
                }
            }

            void Update() override {}

            bool checkRequirements() override {
                return target->hasComponent<CO::KinematicsComponent>() && target->hasComponent<CO::TransformComponent>(); // Check if the target entity has a kinematics component and a transform component 
            }

            std::unique_ptr<Component> clone() const override {
                return std::make_unique<KinematicsControllerBehavior>(*this); // Clone the behavior
            }
    };
}