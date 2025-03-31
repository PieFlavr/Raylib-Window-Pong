/**
 * @file oriented_kinematics_behavior.hpp
 * @author For constraining an Orientated Kinematics Component's speed, turning, and etc.
 * @brief 
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "behavior.hpp"
#include "entity.hpp"
#include "component.hpp"

#include "kinematics_component.hpp"
#include "oriented_kinematics_component.hpp"

namespace CO{
    class OrientedKinematicsBehavior : public Behavior {
        protected:
            float xzHeadingTurnSpeed = 0.05f; // Speed of the entity's rotation in the XZ plane
            float yzHeadingTurnSpeed = 0.05f; // Speed of the entity's rotation in the YZ plane
            float max_speed = 5.0f; // Maximum speed of the entity
            float boost_acceleration = 1.0f; // Acceleration of the entity when boosting

            //=================================================================================================================
            /// BEHAVIOR DATA
            //=================================================================================================================
            float targetSpeed = 0.0f; // Target speed of the entity
            float targetXYHeading = 0.0f; // Target heading of the entity in the XY plane
            float targetYZHeading = 0.0f; // Target heading of the entity in the YZ plane

        public:
            OrientedKinematicsBehavior() = default;
            OrientedKinematicsBehavior(Entity* target) : Behavior(target) { checkRequirements(); }
            OrientedKinematicsBehavior(Entity* target, float turnSpeed, float max_speed, float boost_acceleration) :
            Behavior(target), xzHeadingTurnSpeed(turnSpeed), yzHeadingTurnSpeed(turnSpeed), max_speed(max_speed), boost_acceleration(boost_acceleration) { checkRequirements(); }
            OrientedKinematicsBehavior(Entity* target, float xzHeadingTurnSpeed, float yzHeadingTurnSpeed, float max_speed, float boost_acceleration) : 
            Behavior(target), xzHeadingTurnSpeed(xzHeadingTurnSpeed), yzHeadingTurnSpeed(yzHeadingTurnSpeed), max_speed(max_speed), boost_acceleration(boost_acceleration) {
                checkRequirements();
            }
            OrientedKinematicsBehavior(const OrientedKinematicsBehavior& other) = default;
            OrientedKinematicsBehavior& operator=(const OrientedKinematicsBehavior& other) = default;

            void Tick(const float& deltaTime) override {
                if(enabled){
                    auto orientedKinematicsComponent = (target->getComponent<CO::OrientedKinematicsComponent>());
                    if(orientedKinematicsComponent){
                        float speed = orientedKinematicsComponent.value().get().getHeadSpeed(); // Get the current speed of the entity
                        float xzHeading = orientedKinematicsComponent.value().get().getXZHeading(); // Get the current heading of the entity in the XZ plane
                        float yzHeading = orientedKinematicsComponent.value().get().getYZHeading(); // Get the current heading of the entity in the YZ plane

                        // Update the target speed based on the current speed and acceleration
                        if(std::fabs(speed - targetSpeed) < 0.01f) orientedKinematicsComponent.value().get().setHeadSpeed(targetSpeed); // Snap to the target speed
                        else{
                            if(speed < targetSpeed) orientedKinematicsComponent.value().get().setHeadAcceleration(boost_acceleration); // Accelerate towards the target speed
                            else if(speed > targetSpeed) orientedKinematicsComponent.value().get().setHeadAcceleration(-boost_acceleration); // Decelerate towards the target speed
                        }

                        // Clamp the speed to the maximum speed
                        if(speed > max_speed) orientedKinematicsComponent.value().get().setHeadSpeed(max_speed); // Clamp the speed to the maximum speed
                        else if(speed < -max_speed) orientedKinematicsComponent.value().get().setHeadSpeed(-max_speed); // Clamp the speed to the maximum speed

                        // Update the target heading based on the current heading and turn speed
                        if (std::fabs(targetXYHeading - xzHeading) < 0.01f) {
                            orientedKinematicsComponent.value().get().setXZHeading(targetXYHeading); // Snap to the target heading
                        } else {
                            if(xzHeading < targetXYHeading) orientedKinematicsComponent.value().get().setXZHeading(xzHeading + xzHeadingTurnSpeed * deltaTime); // Turn towards the target heading
                            else if(xzHeading > targetXYHeading) orientedKinematicsComponent.value().get().setXZHeading(xzHeading - xzHeadingTurnSpeed * deltaTime); // Turn towards the target heading    
                        }

                        if(std::fabs(targetYZHeading - yzHeading) < 0.01f) {
                            orientedKinematicsComponent.value().get().setYZHeading(targetYZHeading); // Snap to the target heading
                        } else {
                            if(yzHeading < targetYZHeading) orientedKinematicsComponent.value().get().setYZHeading(yzHeading + yzHeadingTurnSpeed * deltaTime); // Turn towards the target heading
                            else if(yzHeading > targetYZHeading) orientedKinematicsComponent.value().get().setYZHeading(yzHeading - yzHeadingTurnSpeed * deltaTime); // Turn towards the target heading
                        }


                    }
                }
            }

            void Update() override {} // No update logic for this component

            //=================================================================================================================
            /// BEHAVIORs
            //=================================================================================================================
            void incTargetXZHeading(float amount) { targetXYHeading += amount; } // Increase the target heading in the XZ plane
            void incTargetYZHeading(float amount) { targetYZHeading += amount; } // Increase the target heading in the YZ plane
            void incTargetSpeed(float amount) { targetSpeed += amount; } // Increase the target speed of the entity

            void setTargetHeadings(float targetXZHeading, float targetYZHeading) { this->targetXYHeading = targetXZHeading; this->targetYZHeading = targetYZHeading; } // Set the target heading in the XZ and YZ planes
            void setTargetXZHeading(float targetXZHeading) { this->targetXYHeading = targetXZHeading; } // Set the target heading in the XZ plane
            void setTargetYZHeading(float targetYZHeading) { this->targetYZHeading = targetYZHeading; } // Set the target heading in the YZ plane
            void setTargetSpeed(float targetSpeed) { this->targetSpeed = targetSpeed; } // Set the target speed of the entity

            //=================================================================================================================
            /// ACCESSORs
            //=================================================================================================================
            float getXZHeadingTurnSpeed() const { return xzHeadingTurnSpeed; }
            float getYZHeadingTurnSpeed() const { return yzHeadingTurnSpeed; }
            float getMaxSpeed() const { return max_speed; }
            float getBoostAcceleration() const { return boost_acceleration; }
            float getTargetSpeed() const { return targetSpeed; }

            //=================================================================================================================
            /// MUTATORs
            //=================================================================================================================
            void setXZHeadingTurnSpeed(float xzHeadingTurnSpeed) { this->xzHeadingTurnSpeed = xzHeadingTurnSpeed; }
            void setYZHeadingTurnSpeed(float yzHeadingTurnSpeed) { this->yzHeadingTurnSpeed = yzHeadingTurnSpeed; }
            void setMaxSpeed(float max_speed) { this->max_speed = max_speed; }
            void setBoostAcceleration(float boost_acceleration) { this->boost_acceleration = boost_acceleration; }

            //=================================================================================================================
            /// CHECK LOGIC
            //=================================================================================================================
            bool checkRequirements() override { 
                if(!target->hasComponent<CO::OrientedKinematicsComponent>()){
                    throw std::runtime_error("OrientedKinematicsBehavior requires an OrientedKinematicsComponent");
                    return false; 
                }
                return true;
            }

            std::unique_ptr<Component> clone() const override { return std::make_unique<OrientedKinematicsBehavior>(*this); }
    };
}