/**
 * @file follow_camera_behavior.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <iostream>

#include "behavior.hpp"
#include "camera_component.hpp"
#include "transform_component.hpp"

namespace CO{

    class FollowCameraBehavior : public Behavior {
        private:
            Vector3 targetOffset = {0, 60, 200}; // Default target offset
            float lerpFactor = 0.1f; // Default lerp factor

        public:
            FollowCameraBehavior() = default;
            FollowCameraBehavior(Entity* target, const Vector3& targetOffset, const float& lerpFactor) : Behavior(target), targetOffset(targetOffset), lerpFactor(lerpFactor) {
                checkRequirements();
            }

            void Tick(const float& deltaTime) override {
                if(enabled){
                    if (target->hasComponent<CO::CameraComponent>() && target->hasComponent<CO::TransformComponent>()) {
                        auto camera = target->getComponent<CO::CameraComponent>();
                        Vector3 targetPosition = Vector3Add(target->getComponent<CO::TransformComponent>().value().get().getPosition(), targetOffset);

                        camera->get().setPosition(Vector3Lerp(camera->get().getPosition(), targetPosition, (1-std::exp(-lerpFactor * deltaTime)))); // Smoothly move the camera to the target position
                        camera->get().setTarget(Vector3Lerp(camera->get().getTarget(), target->getComponent<CO::TransformComponent>().value().get().getPosition(), (1-std::exp(-lerpFactor * deltaTime))));
                    } 
                }
            }

            void Update() override {}
            
            bool checkRequirements() override {
                if (target->hasComponent<CO::CameraComponent>() == false) {
                    throw std::runtime_error("FollowCameraBehavior requires a CameraComponent.");
                } else if (target->hasComponent<CO::TransformComponent>() == false) {
                    throw std::runtime_error("FollowCameraBehavior requires a TransformComponent.");
                }
                return true;
            }
            std::unique_ptr<Component> clone() const override { return std::make_unique<FollowCameraBehavior>(*this); }
    };
} // namespace CO