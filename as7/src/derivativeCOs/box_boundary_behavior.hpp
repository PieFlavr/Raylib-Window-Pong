/**
 * @file box_boundary_behavior.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

// #include <iostream>

#include "raylib.h"

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

#include "box_collision_component.hpp"
#include "kinematics_component.hpp"
#include "utils.cpp"

namespace CO{
    /**
     * @brief Solely for keeping box collisions in a specific area.
     * 
     */
    class BoxBoundaryBehavior : public Behavior {
        protected:
            Rectangle worldBoundary; // World boundary for the box containment

        public:
            BoxBoundaryBehavior() = delete; // Delete the default constructor to prevent instantiation without parameters
            BoxBoundaryBehavior(Entity* target, Rectangle worldBoundary) : Behavior(target), worldBoundary(worldBoundary) {} // Constructor taking in a target entity and a world boundary
            BoxBoundaryBehavior(const BoxBoundaryBehavior& other) : Behavior(other), worldBoundary(other.worldBoundary) {} // Copy constructor
            BoxBoundaryBehavior& operator=(const BoxBoundaryBehavior& other) { // Copy assignment operator
                if (this != &other) { // Check for self-assignment
                    Behavior::operator=(other); // Call the base class assignment operator
                    worldBoundary = other.worldBoundary; // Copy the world boundary
                }
                return *this; // Return the current object
            }

            void Tick(const float& deltaTime) override { // Tick function to be called every frame
                if(enabled){ // Check if the behavior is enabled
                    if(target->hasComponent<CO::BoxCollisionComponent>() && target->hasComponent<CO::KinematicsComponent>()) { // Check if the target entity has a box collision component and kinematics component
                        Rectangle targetRect = target->getComponent<CO::BoxCollisionComponent>()->get().get2DRectangle(); // Get the target entity's rectangle
                        KinematicsComponent& kinematics = target->getComponent<CO::KinematicsComponent>()->get(); // Get the target entity's kinematics component

                        Vector3 targetPosition = kinematics.getPosition(); // Get the target entity's position

                        // std::cout << "Target Position: " << targetPosition.x << ", " << targetPosition.y << std::endl; // Print the target position
                        // std::cout << "World Boundary: " << worldBoundary.x << ", " << worldBoundary.y << ", " << worldBoundary.width << ", " << worldBoundary.height << std::endl; // Print the world boundary
                        // std::cout << "Target Rect: " << targetRect.x << ", " << targetRect.y << ", " << targetRect.width << ", " << targetRect.height << std::endl; // Print the target rectangle

                        targetPosition.x = std::fmax(worldBoundary.x, std::fmin(targetPosition.x, worldBoundary.x + worldBoundary.width - targetRect.width)); // Clamp the x position to the world boundary
                        targetPosition.y = std::fmax(worldBoundary.y, std::fmin(targetPosition.y, worldBoundary.y + worldBoundary.height - targetRect.height)); // Clamp the y position to the world boundary
                        
                        // std::cout << "Clamped Position: " << targetPosition.x << ", " << targetPosition.y << std::endl; // Print the clamped position

                        kinematics.setPosition(targetPosition); // Set the target entity's position to the clamped position
                        target->getComponent<CO::BoxCollisionComponent>()->get().setLocus(targetPosition); // Set the collision locus to the clamped position
                    } 
                }
            }

            void Update() override {}

            std::unique_ptr<Component> clone() const override { // Clone the behavior
                return std::make_unique<BoxBoundaryBehavior>(*this); // Return a new instance of the behavior
            }

            bool checkRequirements() override {
                if (!target->hasComponent<CO::BoxCollisionComponent>() || !target->hasComponent<CO::KinematicsComponent>()) throw std::runtime_error("BoxBoundaryBehavior requires a BoxCollisionComponent and KinematicsComponent"); // Check if the target entity has a box collision component and kinematics component
                return true; // Return true if the requirements are met
            } 
    };
}