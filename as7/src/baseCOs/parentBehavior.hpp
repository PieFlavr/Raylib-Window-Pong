/**
 * @file parentBehavior.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <vector>
#include <memory>

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

namespace CO{
    class ParentBehavior : public Behavior {
        protected:
            //The target in this case is the "paret"
            std::vector<std::shared_ptr<CO::Entity>> children; // Vector of child entities

        public:
            ParentBehavior() = delete; // Delete the default constructor to prevent instantiation without parameters
            ParentBehavior(Entity* target) : Behavior(target) {} // Constructor taking in a target entity

            void Tick(const float& deltaTime) override {
                for(auto& child : children){ // Iterate through each child entity
                    child->Tick(deltaTime); // Call the Tick function on each child entity
                }
            }

            void Update() override {
                for(auto& child : children){ // Iterate through each child entity
                    child->Update(); // Call the Update function on each child entity
                }
            }

            bool checkRequirements() override { return true; } 

            std::unique_ptr<Component> clone() const override {
                return std::make_unique<ParentBehavior>(*this); // Clone the behavior
            }

    };
}