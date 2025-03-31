/**
 * @file window_kinematics_behavior.hpp
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
#include "window_component.hpp"
#include "window_render_component.hpp"

#include "transform_component.hpp"

namespace CO{
    /**
     * @brief Requires a WindowComponent, WindowRenderComponent, and TransformComponent before being added.
     *          Syncs the above components with the target entity's transform component.
     * 
     */
    class WindowBindBehavior : public Behavior {

        public:
            WindowBindBehavior(Entity* target) : Behavior(target) { // Constructor taking in a target entity
                // Create a new transform component from the target entity's transform component
                if(!checkRequirements()) throw std::runtime_error("WindowBindBehavior requires a WindowComponent"); // Check if the target entity has a window component
                target->getComponent<CO::WindowRenderComponent>()->get().setWindowId(target->getComponent<CO::WindowComponent>()->get().getWindowId()); // Set the window render component's window ID to the window component's window ID
            }

            void Tick(const float& deltaTime) override {
                if(enabled){ //Syncs the transform component with the windows.
                    if(target->hasComponent<CO::WindowComponent>()) { // Check if the target entity has a window component
                        Vector3 transform_pos = target->getComponent<CO::TransformComponent>()->get().getPosition();
                        Vector3 transform_scale = target->getComponent<CO::TransformComponent>()->get().getScale();

                        target->getComponent<CO::WindowComponent>()->get().setWindowPos({transform_pos.x, transform_pos.y}); // Set the window component's position to the transform component's position
                        target->getComponent<CO::WindowComponent>()->get().setWindowDim({transform_scale.x, transform_scale.y}); // Set the window component's dimensions to the transform component's scale
                    } 
                }
            }

            void Update() override {}

            bool checkRequirements() override {
                return target->hasComponent<CO::WindowComponent>() && target->hasComponent<CO::TransformComponent>() && target->hasComponent<CO::WindowRenderComponent>(); // Check if the target entity has a window component and a transform component 
                // Check if the target entity has a window component and a transform component
            }

            std::unique_ptr<Component> clone() const override {
                return std::make_unique<WindowBindBehavior>(*this); // Create a new instance of WindowBindBehavior with the same parameters
            }
    };
}