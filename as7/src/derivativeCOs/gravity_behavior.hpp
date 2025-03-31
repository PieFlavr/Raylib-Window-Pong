/**
 * @file gravity_behavior.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib.h"
#include "raymath.h"
#include <cmath>

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

#include "kinematics_component.hpp"

namespace CO{
    class GravityBehavior : public Behavior {
        protected:
            float gravity = -9.81f; // Gravity acceleration value
            float drag = 0.8f; // Drag value
        public:
            GravityBehavior(Entity* target) : Behavior(target) { // Constructor taking in a target entity
                if(!checkRequirements()) throw std::runtime_error("GravityBehavior requires a KinematicsComponent"); // Check if the target entity has a kinematics component
            }
            GravityBehavior(const GravityBehavior& other) : Behavior(other), gravity(other.gravity), drag(other.drag) {
                if(!checkRequirements()) throw std::runtime_error("GravityBehavior requires a KinematicsComponent"); // Check if the target entity has a kinematics component
            } 
            GravityBehavior& operator=(const GravityBehavior& other) { // Copy assignment operator
                if (this != &other) { // Check for self-assignment
                    Behavior::operator=(other); // Call the base class assignment operator
                    gravity = other.gravity; // Copy the gravity value
                    drag = other.drag; // Copy the drag value
                }
                return *this; // Return the current object
            }
            

            void Tick(const float& deltaTime) override {
                if(enabled){ // Apply gravity to the target entity's kinematics component
                    if(target->hasComponent<KinematicsComponent>()) { // Check if the target entity has a kinematics component
                        KinematicsComponent& kinematics = target->getComponent<KinematicsComponent>()->get(); // Get the kinematics component
                        Vector3 newVelocity = kinematics.getVelocity(); // Get the current velocity
                        newVelocity.y += gravity * deltaTime; // Apply gravity to the y-component of the velocity
                        newVelocity = Vector3Multiply(newVelocity, {pow(drag, deltaTime), pow(drag, deltaTime), 1.0f}); // Apply drag to the velocity
                        kinematics.setVelocity(newVelocity); // Set the new velocity to the kinematics component
                    } 
                }
            }

            void Update() override {}

            void setGravity(float newGravity) { gravity = newGravity; } // Set the gravity value
            float getGravity() const { return gravity; } // Get the gravity value
            void setDrag(float newDrag) { drag = newDrag; } // Set the drag value
            float getDrag() const { return drag; } // Get the drag value

            bool checkRequirements() override {
                return target->hasComponent<CO::KinematicsComponent>(); // Check if the target entity has a kinematics component 
            }

            std::unique_ptr<Component> clone() const override {
                return std::make_unique<GravityBehavior>(*this); // Clone the behavior
            }
    };
}