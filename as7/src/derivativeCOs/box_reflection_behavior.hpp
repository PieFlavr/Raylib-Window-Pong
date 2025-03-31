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
                        isColliding = {0, 0}; // Reset the collision vector
                        isCollidingBoundary = {0, 0}; // Reset the collision vector for the world boundary

                        KinematicsComponent& kinematics = target->getComponent<CO::KinematicsComponent>()->get(); // Get the target entity's kinematics component

                        Rectangle targetRect = target->getComponent<CO::BoxCollisionComponent>()->get().get2DRectangle(); // Get the target entity's rectangle
                        Vector2 targetVelocity = {target->getComponent<CO::KinematicsComponent>()->get().getVelocity().x, target->getComponent<CO::KinematicsComponent>()->get().getVelocity().y}; // Get the target entity's velocity
                        Vector2 targetPosition = {target->getComponent<CO::KinematicsComponent>()->get().getPosition().x, target->getComponent<CO::KinematicsComponent>()->get().getPosition().y}; // Get the target entity's position
                        
                        kinematics.setEnabled(true); // Enable the kinematics component to allow it to update the position

                            kinematics.Tick(deltaTime); // Call the Tick function to update the position
                            Vector2 nextPosition = {target->getComponent<CO::KinematicsComponent>()->get().getPosition().x, target->getComponent<CO::KinematicsComponent>()->get().getPosition().y}; // Get the target entity's position
                            Rectangle nextRect = {nextPosition.x, nextPosition.y, targetRect.width, targetRect.height}; // Create a rectangle for the next position of the target entity
                        
                        kinematics.setEnabled(false); // Disable the kinematics component to prevent it from updating the position again

                        Vector2 collision_vector = {1.0f, 1.0f}; // Default: no collision (velocity remains 1)

                        collision_vector = CheckCollisionBoundary(nextRect, worldBoundary, targetVelocity); // Check for collision with the world boundary
                        isCollidingBoundary = {(collision_vector.x < 0) ? 1.0f : 0, (collision_vector.y < 0) ? 1.0f : 0}; // Set the collision vector for the world boundary
                        // std::cout << "Collision Vector: " << collision_vector.x << ", " << collision_vector.y << std::endl; // Print the collision vector
                        // std::cout << "Target Position: " << targetPosition.x << ", " << targetPosition.y << std::endl; // Print the target position
                        for(auto& entity : *collisionEntities) { // Loop through the collision entities
                            if(entity->hasComponent<CO::BoxCollisionComponent>()) { // Check if the entity has a box collision component and kinematics component
                                if(entity.get() != target) {
                                    Rectangle entityRect = entity->getComponent<CO::BoxCollisionComponent>()->get().get2DRectangle(); // Get the entity's rectangle
                                    Vector2 current_collision = CheckCollisionBoxPro(nextRect, entityRect, targetVelocity); // Check for collision with the entity
                                    // std::cout << "Entity Rectangle: " << entityRect.x << ", " << entityRect.y << ", " << entityRect.width << ", " << entityRect.height << std::endl; // Print the entity rectangle
                                    Vector2 currentlyColliding = {(current_collision.x < 0) ? 1.0f : 0.0f, (current_collision.y < 0) ? 1.0f : 0.0f}; // Check if the entity is colliding with the target entity
                                    // std::cout << "Currently Colliding: " << currentlyColliding.x << ", " << currentlyColliding.y << std::endl; // Print the currently colliding vector
                                    if(currentlyColliding.x > 0.0f || currentlyColliding.y > 0.0f) {
                                        std::cout << "Collision Vector: " << current_collision.x << ", " << current_collision.y << std::endl; // Print the collision vector
                                        collision_vector = Vector2Multiply(current_collision, collision_vector); // Check for collision with the entity and multiply the collision vector
                                        
                                    }
                                }
                        }

                        isColliding = {(collision_vector.x != 1.0f) ? 1.0f : 0, (collision_vector.y != 1.0f) ? 1.0f : 0}; // Set the collision vector for the entity
                        }

                        target->getComponent<CO::KinematicsComponent>()->get().setPosition({nextPosition.x, nextPosition.y, 0}); // Set the target entity's position to the next position
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