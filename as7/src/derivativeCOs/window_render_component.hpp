/**
 * @file window_render_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib.h"
#include "render_component.hpp"

namespace CO{
    /**
     * @brief A modified render component for specific window rendering.
     * 
     */
    class WindowRenderComponent : public RenderComponent {
        
        protected:
            int defaultWindowId;
            int windowId;

        public:

            //=================================================================================================================
            // CONSTRUCTOR/COPY LOGIC
            //=================================================================================================================
            WindowRenderComponent() = delete;
            WindowRenderComponent(int windowId, std::shared_ptr<Model> model) : RenderComponent(model), windowId(windowId) {}
            WindowRenderComponent(int windowId, std::shared_ptr<Model> model, CO::TransformComponent transform) : RenderComponent(model, transform), windowId(windowId) {}
            WindowRenderComponent(int windowId, std::shared_ptr<Model> model, CO::TransformComponent transform, CO::TransformComponent baseTransform) : RenderComponent(model, transform, baseTransform), windowId(windowId) {}
            WindowRenderComponent(const WindowRenderComponent& other) = default;
            WindowRenderComponent& operator=(const WindowRenderComponent& other) = default;

            //=================================================================================================================
            // RENDERING LOGIC
            //=================================================================================================================
            virtual void Render() override {
                SetActiveWindowContext(windowId); // Set the active window context to the specific window
                RenderComponent::Render(); // Call the base class render function
                SetActiveWindowContext(defaultWindowId); // Reset the active window context to the default window
            }
    };
}