/**
 * @file image_window_render_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib.h"
#include "image_render_component.hpp"

namespace CO{
    class ImageWindowRenderComponent : public ImageRenderComponent {
        protected:
            int windowId; // ID of the window

        public:
            ImageWindowRenderComponent() = delete; // Delete the default constructor to prevent instantiation without parameters
            ImageWindowRenderComponent(int windowId, std::shared_ptr<Texture2D> image, Vector2 position, Vector2 size) : ImageRenderComponent(image, position, size), windowId(windowId) {} // Constructor taking in an image, position, and size
            ImageWindowRenderComponent(const ImageWindowRenderComponent& other) = default; // Copy constructor
            ImageWindowRenderComponent& operator=(const ImageWindowRenderComponent& other) = default; // Copy assignment operator

            void Render() override { 
                SetActiveWindowContext(windowId); // Set the active window context to the specific window
                ImageRenderComponent::Render(); // Call the base class render function
            }
    };
}