/**
 * @file component.hpp
 * @author lucas Pinto
 * @brief A re-implementation with nearly the same behavior of the given "CO.hpp" file, but without use of structs.
 *          NOTE: this was mostly since i don't understand the original "CO.hpp" file that well, and I'm infected by Java and OO design.
 *              also i don't understand structs that well (￣y▽,￣)╭  (i was born in Java) 
 * @version 0.1
 * @date 2025-03-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib-cpp.hpp"

#include <memory>

namespace CO{
    class Component {
        //Don't want components to know about larger entity implicitly
        //Should be a design decision that must be explicitly made later
        //Might make things more tedious but also more explicit, clear and modular.
    
        public:
            bool enabled = true;

            Component() = default;
            Component(bool enabled): enabled(enabled) {}
    
            virtual void Tick(const float& deltaTime) = 0; //For tick-dependent updates
            virtual void Update() = 0; //For tick-independent updates
            virtual std::unique_ptr<Component> clone() const = 0; //For cloning the component

            bool isEnabled() const { return enabled; }
            void setEnabled(bool enabled) { this->enabled = enabled; }

            template<std::derived_from<Component> T>		
            T& as() { return *dynamic_cast<T*>(this); }
    
            virtual ~Component() = default;
    };
}
