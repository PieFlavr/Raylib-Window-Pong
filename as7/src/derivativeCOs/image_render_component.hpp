/**
 * @file image_render_component.hpp
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

#include "component.hpp"
#include "entity.hpp"

namespace CO{
    class ImageRenderComponent : public Component {
        protected:
            std::shared_ptr<Texture2D> image; // Image to be rendered

            Vector2 position; // Position of the image
            Vector2 size; // Size of the image

        public:
            ImageRenderComponent() = delete; // Delete the default constructor to prevent instantiation without parameters
            ImageRenderComponent(std::shared_ptr<Texture2D> image, Vector2 position, Vector2 size) : Component(), image(image), position(position), size(size) {} // Constructor taking in an image, position, and size
            ImageRenderComponent(const ImageRenderComponent& other) = default; // Copy constructor
            ImageRenderComponent& operator=(const ImageRenderComponent& other) = default; // Copy assignment operator

            void Tick(const float& deltaTime) override {} // Tick function (not used for rendering)
            void Update() override {} // Update function (not used for rendering)

            std::unique_ptr<Component> clone() const override { return std::make_unique<ImageRenderComponent>(*this); } // Clone function

            virtual void Render() { DrawTexture(*image, 200, 200, WHITE); } // Render function to draw the image at the specified position and size
    };
}