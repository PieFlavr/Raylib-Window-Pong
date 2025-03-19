/**
 * @file transform_component.hpp
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
#include "raymath.h"

namespace CO{
    class TransformComponent : public Component {
        protected:
            raylib::Matrix transformMatrix = MatrixIdentity(); // Default to identity matrix

        public: 
            TransformComponent() = default;
            TransformComponent(const raylib::Matrix& transformMatrix) : transformMatrix(transformMatrix) {}
            TransformComponent(const TransformComponent& other) : transformMatrix(other.transformMatrix) {}
            TransformComponent& operator=(const TransformComponent& other) {
                if (this != &other) {
                    transformMatrix = other.transformMatrix;
                }
                return *this;
            }
            TransformComponent(const Vector3& position, const Vector3& scale, const Quaternion& rotation) {
                transformMatrix = MatrixScale(scale.x, scale.y, scale.z);
                transformMatrix = MatrixMultiply(transformMatrix, QuaternionToMatrix(rotation));
                transformMatrix = MatrixMultiply(transformMatrix, MatrixTranslate(position.x, position.y, position.z));
            }

            void Tick(const float& deltaTime) override {}
            void Update() override {}
            std::unique_ptr<Component> clone() const override { return std::make_unique<TransformComponent>(*this); }

            //=================================================================================================================
            // ACCESSORs
            //=================================================================================================================

            Matrix getTransformMatrix() const { return transformMatrix; }
            Vector3 getPosition() const { return Vector3{transformMatrix.m12, transformMatrix.m13, transformMatrix.m14};}
            Vector3 getScale() const {
                return Vector3{
                    Vector3Length({transformMatrix.m0, transformMatrix.m1, transformMatrix.m2}),
                    Vector3Length({transformMatrix.m4, transformMatrix.m5, transformMatrix.m6}),
                    Vector3Length({transformMatrix.m8, transformMatrix.m9, transformMatrix.m10})};
            }
            Matrix getRotationMatrix() const {
                Vector3 scale = this->getScale();
                Matrix rotationMatrix = transformMatrix;
                rotationMatrix.m0 /= scale.x; rotationMatrix.m1 /= scale.x; rotationMatrix.m2 /= scale.x;
                rotationMatrix.m4 /= scale.y; rotationMatrix.m5 /= scale.y; rotationMatrix.m6 /= scale.y;
                rotationMatrix.m8 /= scale.z; rotationMatrix.m9 /= scale.z; rotationMatrix.m10 /= scale.z;
        
                return rotationMatrix;
            }
            Quaternion getQuaternionRotation() const { return QuaternionFromMatrix(getRotationMatrix()); }
            Vector3 getEulerRotation() const { return QuaternionToEuler(getQuaternionRotation()); }

            //=================================================================================================================
            // MUTATORs
            //=================================================================================================================

            void setTransformMatrix(Matrix transformMatrix) { this->transformMatrix = transformMatrix; }
            void setPosition(Vector3 position) { transformMatrix.m12 = position.x; transformMatrix.m13 = position.y; transformMatrix.m14 = position.z; }
            void setScale(Vector3 scale){
                Vector3 oldScale = this->getScale();
                if(oldScale.x != 0 && scale.x != 0){
                    transformMatrix.m0 *= scale.x / oldScale.x; transformMatrix.m1 *= scale.x / oldScale.x; transformMatrix.m2 *= scale.x / oldScale.x;
                }
                if(oldScale.y != 0 && scale.y != 0){
                    transformMatrix.m4 *= scale.y / oldScale.y; transformMatrix.m5 *= scale.y / oldScale.y; transformMatrix.m6 *= scale.y / oldScale.y;
                }
                if(oldScale.z != 0 && scale.z != 0){
                    transformMatrix.m8 *= scale.z / oldScale.z; transformMatrix.m9 *= scale.z / oldScale.z; transformMatrix.m10 *= scale.z / oldScale.z;
                }
            }
            void setQuaternionRotation(Quaternion rotation){
                Vector3 scale = this->getScale();
                Vector3 position = this->getPosition();

                Matrix rotationMatrix = QuaternionToMatrix(rotation);

                transformMatrix = MatrixIdentity();
                
                transformMatrix = MatrixMultiply(transformMatrix, MatrixScale(scale.x, scale.y, scale.z));
                transformMatrix = MatrixMultiply(transformMatrix, rotationMatrix);
                transformMatrix = MatrixMultiply(transformMatrix, MatrixTranslate(position.x, position.y, position.z));
            }
            void setEulerRotation(Vector3 rotation) {setQuaternionRotation(QuaternionFromEuler(rotation.x, rotation.y, rotation.z)); }

            //For modifying on the go without expensive recalculation when setting
            void multiplyTransformMatrix(Matrix transformMatrix) { this->transformMatrix = MatrixMultiply(this->transformMatrix, transformMatrix); }
            void translate(Vector3 position) { this->transformMatrix = MatrixMultiply(this->transformMatrix, MatrixTranslate(position.x, position.y, position.z)); }
            void scale(Vector3 scale) { this->transformMatrix = MatrixMultiply(this->transformMatrix, MatrixScale(scale.x, scale.y, scale.z)); }
            void quaternionRotate(Quaternion rotation) { this->transformMatrix = MatrixMultiply(this->transformMatrix, QuaternionToMatrix(rotation)); }
            void eulerRotate(Vector3 rotation) { quaternionRotate(QuaternionFromEuler(rotation.x, rotation.y, rotation.z)); }
        };
}