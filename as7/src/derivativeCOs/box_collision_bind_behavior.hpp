/**
 * @file box_collision_bind_behavior.hpp
 * @author 
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib.h"

#include "component.hpp"
#include "entity.hpp"
#include "behavior.hpp"

#include "box_collision_component.hpp"
#include "transform_component.hpp"

namespace CO {
    class BoxCollisionBindBehavior : public Behavior {
        public: 
            BoxCollisionBindBehavior() = default; // Default constructor
            BoxCollisionBindBehavior(Entity* target) : Behavior(target) {} // Constructor taking in a target entity
            BoxCollisionBindBehavior(const BoxCollisionBindBehavior& other) = default; // Copy constructor
            BoxCollisionBindBehavior& operator=(const BoxCollisionBindBehavior& other) = default; // Copy assignment operator

            void Tick(const float& deltaTime) override {
                if (enabled) {
                    if (target->hasComponent<CO::BoxCollisionComponent>() && target->hasComponent<CO::TransformComponent>()) { // Check if the target entity has a box collision component and transform component
                        CO::BoxCollisionComponent& boxCollision = target->getComponent<CO::BoxCollisionComponent>()->get(); // Get the box collision component of the target entity
                        CO::TransformComponent& transform = target->getComponent<CO::TransformComponent>()->get(); // Get the transform component of the target entity

                        boxCollision.setLocus(transform.getPosition()); // Set the collision locus to the transform position
                        boxCollision.setBox(BoundingBox{0, 0, 0, transform.getScale().x, transform.getScale().y, 0}); // Set the box collision to the transform scale
                    }
                }
            } // Tick function to be called every frame
            void Update() override {} // Update function to be called every frame

            std::unique_ptr<Component> clone() const override { return std::make_unique<BoxCollisionBindBehavior>(*this); } // Clone Implement
            ~BoxCollisionBindBehavior() override = default; // Virtual destructor for safety

            bool checkRequirements() override { 
                if (!target->hasComponent<CO::BoxCollisionComponent>() || !target->hasComponent<CO::TransformComponent>()) throw std::runtime_error("BoxCollisionBindComponent requires a BoxCollisionComponent and TransformComponent"); // Check if the target entity has a box collision component and transform component
                return true; // Return true if the requirements are met
            }
    };
}