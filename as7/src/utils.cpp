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

#include <iostream>
#include <cmath>

Vector2 GetRelativePosition(Vector2 world_pos, int window_id, int current_window_id) {
    SetActiveWindowContext(window_id);  

    Vector2 window_pos = GetWindowPosition();  // Get top-left corner of the window
    return (Vector2){ world_pos.x - window_pos.x, world_pos.y - window_pos.y };

    SetActiveWindowContext(current_window_id);
}

Vector2 CheckCollisionBoundary(Rectangle internal_rect, Rectangle boundary_rect, Vector2 velocity) {
    Vector2 collision_vector = {1.0f, 1.0f};  // Default: no collision (velocity remains 1)

    if ((internal_rect.x <= boundary_rect.x && velocity.x < 0) || 
        (internal_rect.x + internal_rect.width >= boundary_rect.width && velocity.x > 0)) {
        collision_vector.x *= -1.0f;
    } 

    if ((internal_rect.y <= boundary_rect.y && velocity.y < 0) || 
        (internal_rect.y + internal_rect.height >= boundary_rect.height && velocity.y > 0)) {
        collision_vector.y *= -1.0f;
    }

    return collision_vector;  // Return -1 for any axis with collision, 1 otherwise
}

bool CheckBoundaryContainment(Vector2 position, Rectangle boundary_rect) {
    return (position.x >= boundary_rect.x && position.x <= boundary_rect.x + boundary_rect.width &&
            position.y >= boundary_rect.y && position.y <= boundary_rect.y + boundary_rect.height);
}

Vector2 CheckCollisionBoxPro(Rectangle object_rect, Rectangle boundary_rect, Vector2 velocity) {

    Vector2 collision_vector = {1.0f, 1.0f};  // Default: no collision (velocity remains 1)

    if (CheckBoundaryContainment((Vector2){object_rect.x, object_rect.y}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x + object_rect.width, object_rect.y}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x, object_rect.y + object_rect.height}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x + object_rect.width, object_rect.y + object_rect.height}, boundary_rect)) {

        if ((object_rect.x <= boundary_rect.x && velocity.x > 0) || 
            (object_rect.x + object_rect.width >= boundary_rect.x + boundary_rect.width && velocity.x < 0)) {
            collision_vector.x *= -1.0f;
        } 

        if ((object_rect.y <= boundary_rect.y && velocity.y > 0) || 
            (object_rect.y + object_rect.height >= boundary_rect.y + boundary_rect.height && velocity.y < 0)) {
            collision_vector.y *= -1.0f;
        }
    }

    return collision_vector;  // Return -1 for any axis with collision, 1 otherwise
}

std::string generateEncodedString(float value, float minValue, float maxValue, int numLines) {
    float percent = (value - minValue) / (maxValue - minValue);
    percent = std::fmax(0.0f, std::fmin(1.0f, percent));
    int fillPosition = std::min((int)(percent * numLines),numLines-1);

    std::string filler = ".";

    std::string encoding = "";

    for(int j = numLines-1; j >= 0 ; j--){
        encoding += (j == fillPosition) ? "#" : ".";
    }

    return encoding;
}

void renderEncodedString(std::string &encoding, int lineLength, int numLines) {
    // Move cursor up numLines to overwrite previous output
    for (int i = 0; i < numLines; i++) {
        std::cout << "\033[A" << std::flush;  
    }

    for (int i = 0 ; i < numLines; i++) { 
        for(int j = lineLength-1; j >= 0; j--){
            if(i + (j * numLines) >= encoding.size()){
                break;
            }
            std::cout << encoding[i + (j * numLines)];
        }
        std::cout << std::endl;
    }
}

#endif // UTILS_CPP