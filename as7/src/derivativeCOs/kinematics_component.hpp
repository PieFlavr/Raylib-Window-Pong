/**
 * @file kinematic_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "component.hpp"

namespace CO{
    
    /**
     * @brief Component storing the kinematic data of an Entity, encoded as velocity and acceleration vectors.
     * 
     */
    class KinematicsComponent : public Component {
        protected:
            Vector3 position = {0.0f, 0.0f, 0.0f};	 //~3D Entity position
            Vector3 velocity = {0.0f, 0.0f, 0.0f};	 //~3D Entity velocity  
            Vector3 acceleration = {0.0f, 0.0f, 0.0f}; //~3D Entity acceleration

        public: 
            void Tick(const float& deltaTime) override {
                // Update the position based on velocity
                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                position.z += velocity.z * deltaTime;

                // Update the velocity based on acceleration
                velocity.x += acceleration.x * deltaTime;
                velocity.y += acceleration.y * deltaTime;
                velocity.z += acceleration.z * deltaTime;

                //std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
            }

            void Update() override {} // No update logic for this component

            //=================================================================================================================
            // CONSTRUCTORS
            //=================================================================================================================
            KinematicsComponent() = default;
            KinematicsComponent(const Vector3& position) : position(position) {}
            KinematicsComponent(const Vector3& position, const Vector3& velocity) : position(position), velocity(velocity) {}
            KinematicsComponent(const KinematicsComponent& other) = default;
            KinematicsComponent& operator=(const KinematicsComponent& other) = default;


            //=================================================================================================================
            // ACCESSORs
            //=================================================================================================================
            Vector3 getPosition() const { return position; }
            Vector3 getVelocity() const { return velocity; }
            Vector3 getAcceleration() const { return acceleration; }
            
            //=================================================================================================================
            // MUTATORs
            //=================================================================================================================
            void setPosition(const Vector3& new_position) { position = new_position; }
            void setVelocity(const Vector3& new_velocity) { velocity = new_velocity; }
            void setAcceleration(const Vector3& new_acceleration) { acceleration = new_acceleration; }
    };
}