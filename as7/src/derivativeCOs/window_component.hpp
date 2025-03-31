/**
 * @file window_component.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "raylib.h"

#include "component.hpp"
#include "entity.hpp"

namespace CO{
    class WindowComponent : public Component {
        protected:
            Vector2 window_dim;
            Vector2 window_pos;
            int window_id; //Cannot be set otherwise.
            std::string window_title; 

        public:
            WindowComponent() = delete; // Delete the default constructor to prevent instantiation without parameters
            WindowComponent(Vector2 window_pos, Vector2 window_dim, std::string window_title) : Component(), window_id(window_id), window_pos(window_pos), window_title(window_title){
                window_id = InitWindowPro(window_dim.x, window_dim.y, window_title.c_str(), 0); // Initialize the window with the given dimensions and title
            }
            WindowComponent(Vector2 window_pos, Vector2 window_dim) : Component(), window_id(window_id), window_pos(window_pos){
                window_id = InitWindowPro(window_dim.x, window_dim.y, "Window", 0); // Initialize the window with the given dimensions and title
            }
            //Can't be copied since windows are a "static", don't want to find out nasty behaviors the hard way for now
            WindowComponent(const WindowComponent& other) = delete; // Delete the copy constructor to prevent copying
            WindowComponent& operator=(const WindowComponent& other) = delete; // Delete the copy assignment operator to prevent copying

            int getWindowId() const { return windowId; }
            Vector2 getWindowPos() const { return window_pos; } // Get the position of the window
            Vector2 getWindowDim() const { return window_dim; } // Get the dimensions of the window
            std::string getWindowTitle() const { return window_title; } // Get the title of the window

            void setWindowPos(Vector2 new_pos) { window_pos = new_pos; } // Set the position of the window
            void setWindowDim(Vector2 new_dim) { window_dim = new_dim; } // Set the dimensions of the window
            void setWindowTitle(std::string new_title) { window_title = new_title; } // Set the title of the window

        private:
            int windowId;
    };
}