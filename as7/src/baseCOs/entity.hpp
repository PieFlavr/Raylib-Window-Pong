/**
 * @file entity.hpp
 * @brief A reimplementation of the given "CO.hpp" file, but without the use of structs.
 *        Notably, additional functionality was added to the Entity class to allow for component management.
 *        As well as making ownership transfer/modification an explicit action rather than the default behavior.
 * @version 0.1
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "raylib-cpp.hpp"
#include "component.hpp"

#include <memory>
#include <vector>
#include <optional>

namespace CO {
    class Entity : public Component {
        protected:
            std::vector<std::unique_ptr<Component>> components = {};

        public:
            //=================================================================================================================
            // CONSTRUCTOR/COPY LOGIC
            //=================================================================================================================
            Entity() = default;
            Entity(const Entity&);
            Entity(Entity&& other); // Move constructor
            Entity& operator=(const Entity&);
            Entity& operator=(Entity&& other); // Move assignment operator
            bool TransferComponents(Entity& other); // Explicit transfer of components to another entity
            std::unique_ptr<Component> clone() const override { return std::make_unique<Entity>(*this); } // Clone Implement

            //=================================================================================================================
            // COMPONENT FUNCTIONS
            //=================================================================================================================

            template<std::derived_from<Component> T, typename... Ts>
            T& addComponent(Ts... args);

            template<std::derived_from<Component> T>
            void pushBackComponent(std::unique_ptr<T> component);

            template<std::derived_from<Component> T>
            std::optional<std::reference_wrapper<T>> getComponent();

            template<std::derived_from<Component> T>
            bool hasComponent() const;

            template<std::derived_from<Component> T>
            std::optional<std::unique_ptr<T>> removeComponent();

            //=================================================================================================================
            // ITERATION LOGIC
            //=================================================================================================================
            void Tick(const float& deltaTime) override;
            void Update() override;

            //=================================================================================================================
            // CLEAN UP
            //=================================================================================================================
            ~Entity();
    };
}

#include "entity.tpp" // Include the template definitions