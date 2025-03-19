/**
 * @file box_collision_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "collision_component.hpp"
#include "raylib-cpp.hpp"

namespace CO{
    class BoxCollisionComponent : public CollisionComponent{
        protected:
            BoundingBox box = {0, 0, 0, 0, 0, 0}; // Box dimensions
        public:
            BoxCollisionComponent() = default;
            BoxCollisionComponent(const BoundingBox& box) : box(box) {}
            BoxCollisionComponent(const BoxCollisionComponent& other) : box(other.box) {}
            BoxCollisionComponent& operator=(const BoxCollisionComponent& other) {
                if (this != &other) {
                    box = other.box;
                }
                return *this;
            }
            
            void Tick(const float& deltaTime) override {}
            void Update() override {}
            
            bool checkCollision(const CollisionComponent& other) const override {
                if (const BoxCollisionComponent* otherBox = dynamic_cast<const BoxCollisionComponent*>(&other)) {
                    return CheckCollisionBoxes(box, otherBox->box); // Check collision with another box
                }
                return false; // No collision if the other component is not a box
            }

            void setBox(const BoundingBox& box) { this->box = box; } // Set the box dimensions
            BoundingBox getBox() const { return box; } // Get the box dimensions
            
            std::unique_ptr<Component> clone() const override { return std::make_unique<BoxCollisionComponent>(*this); } // Clone Implement
            ~BoxCollisionComponent() override = default; // Virtual destructor for safety
    };
}