/**
 * @file collision_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "component.hpp"
#include "entity.hpp"

#include "raylib-cpp.hpp"
#include <any>

namespace CO{
    class CollisionComponent : public Component{
        protected:
            Vector3 locus = {0, 0, 0}; // "Center" of the collision component shape

        public:
            CollisionComponent() = default;
            CollisionComponent(const Vector3& locus) : locus(locus) {}
            CollisionComponent(const CollisionComponent& other) : locus(other.locus) {}
            CollisionComponent& operator=(const CollisionComponent& other) {
                if (this != &other) {
                    locus = other.locus;
                }
                return *this;
            }
            
            void Tick(const float& deltaTime) override {}
            void Update() override {}

            virtual bool checkCollision(const CollisionComponent& other) const = 0; // Pure virtual function to be overridden by derived classes

            void getLocus(Vector3& locus) const { locus = this->locus; } // Get the locus of the collision component
            void setLocus(const Vector3& locus) { this->locus = locus; } // Set the locus of the collision component

            virtual std::unique_ptr<Component> clone() const override = 0;

            virtual ~CollisionComponent() = default; // Virtual destructor for safety
    };
}
