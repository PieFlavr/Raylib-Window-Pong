/**
 * @file box_reflection_behavior.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <iostream>
#include <memory>

#include "raylib.h"

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

#include "box_collision_component.hpp"
#include "kinematics_component.hpp"
#include "utils.cpp"

namespace CO{
    class BoxReflectionBehavior : public Behavior {
        protected:
            std::shared_ptr<std::vector<std::shared_ptr<Entity>>> collisionEntities; // List of entities to check for collisions with
            Rectangle worldBoundary; // World boundary for the box reflection behavior
            Vector2 isColliding = {0, 0}; // 0 = no collision, 1 = collision ; {x,y}
            Vector2 isCollidingBoundary = {0, 0}; // 0 = no collision, 1 = collision ; {x,y}
            
        public:
            BoxReflectionBehavior() = delete; // Delete the default constructor to prevent instantiation without parameters
            BoxReflectionBehavior(Entity* target, std::shared_ptr<std::vector<std::shared_ptr<Entity>>> collisionEntities, Rectangle worldBoundary) : Behavior(target), collisionEntities(collisionEntities), worldBoundary(worldBoundary) { // Constructor taking in a target entity, a list of collision entities, and a world boundary
                if(!checkRequirements()) throw std::runtime_error("BoxReflectionBehavior requires a BoxCollisionComponent and KinematicsComponent"); // Check if the target entity has a box collision component and kinematics component
            }
            BoxReflectionBehavior(const BoxReflectionBehavior& other) : Behavior(other), collisionEntities(other.collisionEntities) {} // Copy constructor
            BoxReflectionBehavior& operator=(const BoxReflectionBehavior& other) { // Copy assignment operator
                if (this != &other) { // Check for self-assignment
                    Behavior::operator=(other); // Call the base class assignment operator
                    collisionEntities = other.collisionEntities; // Copy the collision entities
                }
                return *this; // Return the current object
            }

            void Tick(const float& deltaTime) override { // Tick function to be called every frame
                if(enabled){ // Check if the behavior is enabled
                    if(target->hasComponent<CO::BoxCollisionComponent>() && target->hasComponent<CO::KinematicsComponent>()) { // Check if the target entity has a box collision component and kinematics component
                        Rectangle targetRect = target->getComponent<CO::BoxCollisionComponent>()->get().get2DRectangle(); // Get the target entity's rectangle
                        Vector2 targetVelocity = {target->getComponent<CO::KinematicsComponent>()->get().getVelocity().x, target->getComponent<CO::KinematicsComponent>()->get().getVelocity().y}; // Get the target entity's velocity
                        Vector2 targetPosition = {target->getComponent<CO::KinematicsComponent>()->get().getPosition().x, target->getComponent<CO::KinematicsComponent>()->get().getPosition().y}; // Get the target entity's position
                        
                        Vector2 nextPosition = {targetPosition.x + targetVelocity.x * deltaTime, targetPosition.y + targetVelocity.y * deltaTime}; // Calculate the next position of the target entity
                        Rectangle nextRect = {nextPosition.x, nextPosition.y, targetRect.width, targetRect.height}; // Create a rectangle for the next position of the target entity

                        Vector2 collision_vector = {1.0f, 1.0f}; // Default: no collision (velocity remains 1)

                        collision_vector = CheckCollisionBoundary(nextRect, worldBoundary, targetVelocity); // Check for collision with the world boundary
                        isCollidingBoundary = {0 ? (collision_vector.x != 1.0f) : 1.0f, 0 ? (collision_vector.y != 1.0f) : 1.0f}; // Set the collision vector for the world boundary
                        std::cout << "Collision Vector: " << collision_vector.x << ", " << collision_vector.y << std::endl; // Print the collision vector
                        
                        for(auto& entity : *collisionEntities) { // Loop through the collision entities
                            if(entity->hasComponent<CO::BoxCollisionComponent>() && entity->hasComponent<CO::KinematicsComponent>()) { // Check if the entity has a box collision component and kinematics component
                                Rectangle entityRect = entity->getComponent<CO::BoxCollisionComponent>()->get().get2DRectangle(); // Get the entity's rectangle
                                Vector2 current_collision = CheckCollisionBoundary(nextRect, entityRect, targetVelocity); // Check for collision with the entity
                                collision_vector = Vector2Multiply(current_collision, collision_vector); // Check for collision with the entity and multiply the collision vector
                                Vector2 currentlyColliding = {0 ? (current_collision.x != 1.0f) : 1.0f, 0 ? (current_collision.y != 1.0f) : 1.0f}; // Set the collision vector for the entity
                        }

                        isColliding = {0 ? (collision_vector.x != 1.0f) : 1.0f, 0 ? (collision_vector.y != 1.0f) : 1.0f}; // Set the collision vector for the entity
                        }

                        targetVelocity = Vector2Multiply(targetVelocity, collision_vector); // Multiply the target velocity by the collision vector
                        target->getComponent<CO::KinematicsComponent>()->get().setVelocity({targetVelocity.x, targetVelocity.y, 0}); // Set the target entity's velocity to the new velocity
                        
                    }
                }
            }
            void Update() override {} // Update function to be called every frame

            std::unique_ptr<Component> clone() const override { // Clone the behavior
                return std::make_unique<BoxReflectionBehavior>(*this); // Return a new instance of the behavior
            }


            bool checkRequirements() override {
                return target->hasComponent<CO::BoxCollisionComponent>() && target->hasComponent<CO::KinematicsComponent>(); 
            }

            Vector2 getIsColliding() const { return isColliding; } // Get the collision vector
            Vector2 getIsCollidingBoundary() const { return isCollidingBoundary; } // Get the collision vector for the world boundary
            
    };
}