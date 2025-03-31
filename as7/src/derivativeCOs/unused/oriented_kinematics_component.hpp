/**
 * @file oriented_kinematics_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "kinematics_component.hpp"

namespace CO{
    class OrientedKinematicsComponent : public KinematicsComponent {
        float xzHeading = 0.0f; // Heading of the entity in the XY plane
        float yzHeading = 0.0f; // Heading of the entity in the XZ/YZ plane
        float head_speed = 0.0f; // Speed of the entity
        float head_acceleration = 0.0f; // Acceleration of the entity

        public: 
            
            //=================================================================================================================
            // CONSTRUCTORS
            //=================================================================================================================
            OrientedKinematicsComponent() = default;
            OrientedKinematicsComponent(const Vector3& position) : KinematicsComponent(position) {}
            OrientedKinematicsComponent(const Vector3& position, 
                                        const float& head_speed, const float& head_acceleration, 
                                        const float& xzHeading, const float& yzHeading) :
                                        KinematicsComponent(position), head_speed(head_speed), head_acceleration(head_acceleration), xzHeading(xzHeading), yzHeading(yzHeading) {}

            OrientedKinematicsComponent(const OrientedKinematicsComponent& other) = default;
            OrientedKinematicsComponent& operator=(const OrientedKinematicsComponent& other) = default;
            std::unique_ptr<Component> clone() const override { return std::make_unique<OrientedKinematicsComponent>(*this); }

            //=================================================================================================================
            // ITERATION LOGIC
            //=================================================================================================================
            void Tick(const float& deltaTime) override {
                if(enabled){
                    // Calculate 3D velocity and acceleration based on 1D speed/acceleration and 2D heading
                    head_speed += head_acceleration * deltaTime; // Update speed based on acceleration
                    
                    velocity.x = head_speed * sin(xzHeading) * cos(yzHeading); //cos instead of sin to "flip" axis start since spherical is calc. from zy/zx instead of xy/xz
                    velocity.y = head_speed * -sin(yzHeading);
                    velocity.z = head_speed * cos(xzHeading) * cos(yzHeading);
                    

                    KinematicsComponent::Tick(deltaTime); // Call the base class Tick function
                }
            }

            void Update() override {} // No update logic for this component

            //=================================================================================================================
            // ACCESSORs
            //=================================================================================================================
            float getXZHeading() const { return xzHeading; }
            float getYZHeading() const { return yzHeading; }
            float getHeadSpeed() const { return head_speed; }
            float getHeadAcceleration() const { return head_acceleration; }
            Quaternion getQuaternionRotation() const { 
                Vector3 eulerRotation = getEulerRotation(); // Get the Euler rotation based on heading
                return QuaternionFromEuler(eulerRotation.x, eulerRotation.y, eulerRotation.z);
            } // Quaternion rotation based on heading
            //WHY THE HELL DOES ROTATION HAVE SHIFTED AXES DEFINITIONS FLIPPED THIS TOOK ME TOO LONG TO NOTICE WTFFFFFFFFFF
            Vector3 getEulerRotation() const { return Vector3{yzHeading, xzHeading, 0}; } // Euler rotation based on heading

            //=================================================================================================================
            // MUTATORs
            //=================================================================================================================
            void setXZHeading(float xzHeading) { this->xzHeading = xzHeading; }
            void setYZHeading(float yzHeading) { this->yzHeading = yzHeading; }
            void setEulerRotation(const Vector3& eulerRotation) { this->xzHeading = eulerRotation.x; this->yzHeading = eulerRotation.y; } // Set the Euler rotation based on heading
            void setHeadSpeed(float head_speed) { this->head_speed = head_speed; }
            void setHeadAcceleration(float head_acceleration) { this->head_acceleration = head_acceleration; }

            void rotateXZHeading(float xzHeading) { this->xzHeading += xzHeading; }
            void rotateYZHeading(float yzHeading) { this->yzHeading += yzHeading; }
        };
}