/**
 * @file utils.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef UTILS_CPP
#define UTILS_CPP

#include "raylib.h"
#include "raymath.h"

Vector2 GetRelativePosition(Vector2 world_pos, int window_id, int current_window_id) {
    SetActiveWindowContext(window_id);  

    Vector2 window_pos = GetWindowPosition();  // Get top-left corner of the window
    return (Vector2){ world_pos.x - window_pos.x, world_pos.y - window_pos.y };

    SetActiveWindowContext(current_window_id);
}

Vector2 CheckInternalBoxCollision(Rectangle box1, Rectangle box2) {
    // Initialize the result as {1, 1} (no collision)
    Vector2 result = {1.0f, 1.0f};

    // Check for horizontal (X axis) collision
    if (box1.x < box2.x || box1.x + box1.width > box2.x + box2.width) {
        result.x = -1.0f;  // Flip X velocity if there's a collision on X
    }

    // Check for vertical (Y axis) collision
    if (box1.y < box2.y || box1.y + box1.height > box2.y + box2.height) {
        result.y = -1.0f;  // Flip Y velocity if there's a collision on Y
    }

    // Return the result, either {1, 1} (no collision) or flipped {1, -1} or {-1, 1}
    return result;
}


#endif // UTILS_CPP