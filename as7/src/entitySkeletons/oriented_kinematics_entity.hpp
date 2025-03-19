/**
 * @file oriented_kinematic_entity.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib-cpp.hpp"

#include "entity.hpp"
#include "component.hpp"
#include "behavior.hpp"

#include "render_component.hpp"
#include "transform_component.hpp"
#include "oriented_kinematics_component.hpp"
#include "box_collision_component.hpp"

#include "transform_bind_behavior.hpp"
#include "oriented_kinematics_behavior.hpp"

namespace CO{
    class OrientedKinematicEntity : public Entity {
        public:
            OrientedKinematicEntity() = default;
            OrientedKinematicEntity(std::shared_ptr<raylib::Model> model, const Vector3& baseScale, const Vector3& baseRotation, const float& maxSpeed, const float& maxAcceleration, const float& turnSpeed){
                
                addComponent<CO::OrientedKinematicsComponent, raylib::Vector3>(Vector3{0, 0, 0});
                addComponent<CO::RenderComponent, std::shared_ptr<raylib::Model>>(model);
                addComponent<CO::TransformComponent, raylib::Matrix>(MatrixIdentity());
                addComponent<CO::BoxCollisionComponent>();
                addComponent<CO::OrientedKinematicsBehavior, CO::Entity*>(std::make_shared<Entity>(*this).get(), turnSpeed, maxSpeed, maxAcceleration);
                addComponent<CO::TransformBindBehavior, CO::Entity*>(std::make_shared<Entity>(*this).get()); //this had issues
                //^^^ this took me so damn long to figure out why it wasn't working, goofy unique_pointering issues
                //^^^ making it shared is goated though so whatever :p

                getComponent<CO::RenderComponent>()->get().setScale(baseScale);
                getComponent<CO::RenderComponent>()->get().setBaseEulerRotation(baseRotation);
            }
            OrientedKinematicEntity(const OrientedKinematicEntity& other) = default;
            OrientedKinematicEntity& operator=(const OrientedKinematicEntity& other) = default;


            std::unique_ptr<Component> clone() const override { return std::make_unique<OrientedKinematicEntity>(*this); }
    };
}