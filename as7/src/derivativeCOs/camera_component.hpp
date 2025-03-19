/**
 * @file camera_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "component.hpp"
#include "raylib-cpp.hpp"

namespace CO{
    class CameraComponent : public Component {
        private:
            raylib::Camera camera; // Camera of the entity

        public:
            //=================================================================================================================
            // CONSTRUCTOR/COPY LOGIC
            //=================================================================================================================
            CameraComponent() = default;
            CameraComponent(const raylib::Camera& camera) : camera(camera) {}
            CameraComponent(const CameraComponent& other) : camera(other.camera) {}
            CameraComponent& operator=(const CameraComponent& other) {
                if (this != &other) {
                    camera = other.camera;
                }
                return *this;
            }
            std::unique_ptr<Component> clone() const override { return std::make_unique<CameraComponent>(*this); }

            //=================================================================================================================
            // RENDERING LOGIC
            //=================================================================================================================

            void BeginCameraMode() { camera.BeginMode(); }
            void EndCameraMode() { camera.EndMode(); }

            //=================================================================================================================
            // ITERATION LOGIC
            //=================================================================================================================
            void Tick(const float& deltaTime) override {}
            void Update() override {}

            //=================================================================================================================
            // ACCESSORs
            //=================================================================================================================
            raylib::Camera getCamera() const { return camera; }
            Vector3 getPosition() const { return camera.position; }
            Vector3 getTarget() const { return camera.target; }
            Vector3 getUp() const { return camera.up; }
            float getFov() const { return camera.fovy; }
            int getProjection() const { return camera.projection; }

            //=================================================================================================================
            // MUTATORs
            //=================================================================================================================

            void setCamera(const raylib::Camera& camera) { this->camera = camera; }
            void setPosition(const Vector3& position) { camera.position = position; }
            void setTarget(const Vector3& target) { camera.target = target; }
            void setUp(const Vector3& up) { camera.up = up; }
            void setFov(float fov) { camera.fovy = fov; }
            void setProjection(int projection) { camera.projection = projection; }

            void translatePosition(const Vector3& position) { camera.position = Vector3Add(camera.position, position); }
            void translateTarget(const Vector3& target) { camera.target = Vector3Add(camera.target, target); }
    };
} // namespace CO