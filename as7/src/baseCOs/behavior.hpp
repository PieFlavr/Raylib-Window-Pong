/**
 * @file behavior.hpp
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

namespace CO{

    /**
     * @brief A pure virtual class for behaviors that can be attached to an entity.
     *      Applies constraints to other components of the entity.
     *      Decouples and modularizes logic from base components.
     */
    class Behavior : public Component{
        protected:
            Entity* target; // Target of the behavior
            //std::vector<std::shared_ptr<Component>> requiredComponents; // Required components for the behavior

        public:
            //=================================================================
            /// CONSTRUCTORS/COPY LOGIC
            //=================================================================
            Behavior() = delete; // Should not exist without a target entity
            Behavior(Entity* target) : target(target) {}
            Behavior(const Behavior& other) { target = other.target; }
            Behavior& operator=(const Behavior& other) { if (this != &other) { target = other.target;} return *this;}

            virtual std::unique_ptr<Component> clone() const = 0;

            //=================================================================
            /// ITERATION LOGIC
            //=================================================================
            virtual void Tick(const float& deltaTime) = 0; // Pure virtual function to be overridden
            virtual void Update() = 0; // Pure virtual function to be overridden

            //=================================================================
            /// CHECK LOGIC
            //=================================================================
            virtual bool checkRequirements() = 0; // Pure virtual function to be overridden
            //^^^ intended to check for required components, etc.

            //=================================================================
            /// ACCESSORs
            //=================================================================
            Entity* getTarget() const { return target; }

            //=================================================================
            /// MUTATORs
            //=================================================================
            void setTarget(Entity* target) { this->target = target; }
            
            //=================================================================
            /// CLEANUP
            //=================================================================
            ~Behavior() = default;
    };

}