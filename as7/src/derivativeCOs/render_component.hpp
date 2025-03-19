/**
 * @file render_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "component.hpp"
#include "transform_component.hpp"

#include "raylib-cpp.hpp"

#include "transforms.cpp"

namespace CO{

    /**
     * @brief Component storing the render data of an Entity, encoded as a model and a transform.
     * 
     */
    class RenderComponent : public Component {
        private:
            std::shared_ptr<raylib::Model> model; // Model of the entity
            CO::TransformComponent transform; // Transform of the entity
            CO::TransformComponent baseTransform; // Base transform of the entity, i.e. permanent rotations and etc.

        public:
            bool boundingBoxEnabled = false; 
            bool arrowsEnabled = false; 

            //=================================================================================================================
            // CONSTRUCTOR/COPY LOGIC
            //=================================================================================================================
            RenderComponent() = delete;
            RenderComponent(std::shared_ptr<raylib::Model> model) : model(model) {}
            RenderComponent(std::shared_ptr<raylib::Model> model, CO::TransformComponent transform) : model(model), transform(transform) {}
            RenderComponent(std::shared_ptr<raylib::Model> model, CO::TransformComponent transform, CO::TransformComponent baseTransform) : model(model), transform(transform), baseTransform(baseTransform) {}
            RenderComponent(const RenderComponent& other) = default;
            RenderComponent& operator=(const RenderComponent& other) = default;
            
            
            //=================================================================================================================
            // ITERATION LOGIC
            //=================================================================================================================
            void Tick(const float& deltaTime) override {}
            void Update() override {}
            std::unique_ptr<Component> clone() const override { return std::make_unique<RenderComponent>(*this); }

            //=================================================================================================================
            // RENDERING LOGIC
            //=================================================================================================================

            virtual void Render() {
                if (model) {
                    Matrix backup = model->GetTransform(); // Backup the original transform matrix
                    Matrix compoundTransform = MatrixMultiply(baseTransform.getTransformMatrix(), transform.getTransformMatrix()); // Combine the base and current transforms
                    model->SetTransform(compoundTransform); // Set the model's transform matrix to the transform component's matrix
                    model->Draw({});
                    if (boundingBoxEnabled) {
                        model->GetTransformedBoundingBox().Draw(); // Draw the bounding box of the model
                    }
                    if(arrowsEnabled){
                        Vector3 xArrowEnd = {2.0f, 0.0f, 0.0f};
                        Vector3 yArrowEnd = {0.0f, 2.0f, 0.0f};
                        Vector3 zArrowEnd = {0.0f, 0.0f, 2.0f};
                        xArrowEnd = Vector3Transform(xArrowEnd, transform.getTransformMatrix());
                        yArrowEnd = Vector3Transform(yArrowEnd, transform.getTransformMatrix());
                        zArrowEnd = Vector3Transform(zArrowEnd, transform.getTransformMatrix());

                        DrawLine3D(transform.getPosition(), xArrowEnd, RED);
                        DrawLine3D(transform.getPosition(), yArrowEnd, GREEN);
                        DrawLine3D(transform.getPosition(), zArrowEnd, BLUE);
                    }
                    model->SetTransform(backup); // Restore the original transform matrix
                }
            }

            //=================================================================================================================
            // ACCESSORs
            //=================================================================================================================

            std::shared_ptr<raylib::Model> getModel() const { return model; }

            CO::TransformComponent getCompoundTransform() const { return CO::TransformComponent(MatrixMultiply(baseTransform.getTransformMatrix(), transform.getTransformMatrix())); }
            Vector3 getPosition() const { return getCompoundTransform().getPosition(); }
            Vector3 getScale() const { return getCompoundTransform().getScale(); }
            Quaternion getQuaternionRotation() const { return getCompoundTransform().getQuaternionRotation(); }
            Vector3 getEulerRotation() const { return getCompoundTransform().getEulerRotation(); }
            
            CO::TransformComponent getTransform() const { return transform; }
            Vector3 getTransformPosition() const { return transform.getPosition(); }
            Vector3 getTransformScale() const { return transform.getScale(); }
            Quaternion getTransformQuaternionRotation() const { return transform.getQuaternionRotation(); }
            Vector3 getTransformEulerRotation() const { return transform.getEulerRotation(); }

            CO::TransformComponent getBaseTransform() const { return baseTransform; }
            Vector3 getBasePosition() const { return baseTransform.getPosition(); }
            Vector3 getBaseScale() const { return baseTransform.getScale(); }
            Quaternion getBaseQuaternionRotation() const { return baseTransform.getQuaternionRotation(); }
            Vector3 getBaseEulerRotation() const { return baseTransform.getEulerRotation(); }
            
            BoundingBox getBoundingBox() const { return GetModelBoundingBox(*model); }
            BoundingBox getTransformedBoundingBox() const { 
                Matrix originalTransform = model->GetTransform();
                Matrix compoundTransform = MatrixMultiply(baseTransform.getTransformMatrix(), transform.getTransformMatrix());
                model->SetTransform(compoundTransform);
                BoundingBox transformedBoundingBox = model->GetTransformedBoundingBox();
                model->SetTransform(originalTransform);
                return transformedBoundingBox;
            }

            //=================================================================================================================
            // MUTATORs
            //=================================================================================================================

            void setModel(std::shared_ptr<raylib::Model> model) { this->model = model; }
            void setTransform(CO::TransformComponent transform) { this->transform = transform; }
            void setBaseTransform(CO::TransformComponent baseTransform) { this->baseTransform = baseTransform; }
            
            void setPosition(Vector3 position) { transform.setPosition(position); }
            void setScale(Vector3 scale) { transform.setScale(scale); }
            void setQuaternionRotation(Quaternion rotation) { transform.setQuaternionRotation(rotation); }
            void setEulerRotation(Vector3 rotation) { transform.setEulerRotation(rotation); }

            void setBasePosition(Vector3 position) { baseTransform.setPosition(position); }
            void setBaseScale(Vector3 scale) { baseTransform.setScale(scale); }
            void setBaseQuaternionRotation(Quaternion rotation) { baseTransform.setQuaternionRotation(rotation); }
            void setBaseEulerRotation(Vector3 rotation) { baseTransform.setEulerRotation(rotation); }

            void multiplyTransformMatrix(Matrix transformMatrix) { transform.multiplyTransformMatrix(transformMatrix); }
            void translate(Vector3 translation) { transform.translate(translation); }
            void scale(Vector3 scale) { transform.scale(scale); }
            void quaternionRotate(Quaternion rotation) { transform.quaternionRotate(rotation); }
            void eulerRotate(Vector3 rotation) { transform.eulerRotate(rotation); }

            void multiplyBaseTransformMatrix(Matrix transformMatrix) { baseTransform.multiplyTransformMatrix(transformMatrix); }
            void translateBase(Vector3 translation) { baseTransform.translate(translation); }
            void scaleBase(Vector3 scale) { baseTransform.scale(scale); }
            void quaternionRotateBase(Quaternion rotation) { baseTransform.quaternionRotate(rotation); }
            void eulerRotateBase(Vector3 rotation) { baseTransform.eulerRotate(rotation); }
    };
} // namespace cs381