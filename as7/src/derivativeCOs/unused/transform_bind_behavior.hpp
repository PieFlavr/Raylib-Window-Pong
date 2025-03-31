/**
 * @file t.hpp
 * @author your name (you@domain.com)
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

#include "transform_component.hpp"

#include "kinematics_component.hpp"
#include "collision_component.hpp"
#include "render_component.hpp"
#include "box_collision_component.hpp"
#include "oriented_kinematics_component.hpp"

namespace CO{
    class TransformBindBehavior : public Behavior {

        public:
            TransformBindBehavior(Entity* target) : Behavior(target) { // Constructor taking in a target entity
                // Create a new transform component from the target entity's transform component
                if(!checkRequirements()) throw std::runtime_error("TransformBindBehavior requires a TransformComponent"); // Check if the target entity has a transform component
            }

            void Tick(const float& deltaTime) override { 

                if(enabled){ //Syncing the transform component with the target entity's transform component

                    if(target->hasComponent<KinematicsComponent>()) { // Check if the target entity has a kinematics component
                        target->getComponent<CO::TransformComponent>()->get().setPosition(target->getComponent<KinematicsComponent>()->get().getPosition()); // Set the transform component's position to the kinematics component's position
                    }  

                    if(target->hasComponent<OrientedKinematicsComponent>()) { // Check if the target entity has an oriented kinematics component
                         target->getComponent<CO::TransformComponent>()->get().setQuaternionRotation(target->getComponent<OrientedKinematicsComponent>()->get().getQuaternionRotation()); // Set the transform component's rotation to the oriented kinematics component's rotation
                    }

                    if(target->hasComponent<RenderComponent>()) { // Check if the target entity has a render component
                        target->getComponent<RenderComponent>()->get().setPosition( target->getComponent<CO::TransformComponent>()->get().getPosition()); // Set the render component's position to the transform component's position
                        target->getComponent<RenderComponent>()->get().setQuaternionRotation( target->getComponent<CO::TransformComponent>()->get().getQuaternionRotation()); // Set the render component's rotation to the transform component's rotation
                         target->getComponent<CO::TransformComponent>()->get().setScale(target->getComponent<RenderComponent>()->get().getScale()); // Set the transform component's scale to the render component's scale
                    }
                    
                    if(target->hasComponent<RenderComponent>() && target->hasComponent<BoxCollisionComponent>()) { // Check if the target entity has a box collision component
                        target->getComponent<BoxCollisionComponent>()->get().setBox(target->getComponent<RenderComponent>()->get().getTransformedBoundingBox()); // Set the box collision component's box to the render component's bounding box
                    } //else if //add other possible collision shapes here

                    if(target->hasComponent<CollisionComponent>()) { // Check if the target entity has a collision component
                        target->getComponent<CollisionComponent>()->get().setLocus( target->getComponent<CO::TransformComponent>()->get().getPosition()); // Set the collision component's locus to the transform component's position
                    }

                     target->getComponent<CO::TransformComponent>()->get().Tick(deltaTime); 
                }
            }

            void Update() override { 
                if(enabled){
                     target->getComponent<CO::TransformComponent>()->get().Update(); 
                }
            }

            bool checkRequirements() override { 
                if(target->hasComponent<TransformComponent>()) { // Check if the target entity has a transform component
                    return true; // If it does, return true
                }
                return false;
            }

            std::unique_ptr<Component> clone() const override { return std::make_unique<TransformBindBehavior>(*this); } // Clone Implement
    };
}