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

/**
 * @brief Get the Relative Position object
 * 
 * @param world_pos The position in the world space
 * @param window_id The ID of the target window
 * @param current_window_id The ID of the current active window
 * @return Vector2 The relative position in the target window
 */
Vector2 GetRelativePosition(Vector2 world_pos, int window_id, int current_window_id) {
    SetActiveWindowContext(window_id);  // Switch to the target window context

    Vector2 window_pos = GetWindowPosition();  // Get top-left corner of the target window
    return (Vector2){ world_pos.x - window_pos.x, world_pos.y - window_pos.y };  // Calculate relative position

    SetActiveWindowContext(current_window_id);  // Restore the original window context
}

/**
 * @brief Check for collisions between an internal rectangle and a boundary rectangle.
 * 
 * @param internal_rect The rectangle inside the boundary.
 * @param boundary_rect The boundary rectangle.
 * @param velocity The velocity vector of the internal rectangle.
 * @return Vector2 A vector indicating collision on both x and y (-1 for collision, 1 otherwise).
 */
Vector2 CheckCollisionBoundary(Rectangle internal_rect, Rectangle boundary_rect, Vector2 velocity) {
    Vector2 collision_vector = {1.0f, 1.0f};  // Default: no collision (velocity remains 1)

    // Check horizontal collision with boundary
    if ((internal_rect.x <= boundary_rect.x && velocity.x < 0) || 
        (internal_rect.x + internal_rect.width >= boundary_rect.width && velocity.x > 0)) {
        collision_vector.x *= -1.0f;  // Reverse horizontal velocity
    } 

    // Check vertical collision with boundary
    if ((internal_rect.y <= boundary_rect.y && velocity.y < 0) || 
        (internal_rect.y + internal_rect.height >= boundary_rect.height && velocity.y > 0)) {
        collision_vector.y *= -1.0f;  // Reverse vertical velocity
    }

    return collision_vector;  // Return -1 for any axis with collision, 1 otherwise
}

/**
 * @brief Check if a position is within a boundary rectangle.
 * 
 * @param position The position to check.
 * @param boundary_rect The boundary rectangle.
 * @return true If the position is inside the boundary.
 * @return false If the position is outside the boundary.
 */
bool CheckBoundaryContainment(Vector2 position, Rectangle boundary_rect) {
    return (position.x >= boundary_rect.x && position.x <= boundary_rect.x + boundary_rect.width &&
            position.y >= boundary_rect.y && position.y <= boundary_rect.y + boundary_rect.height);
}

/**
 * @brief Checks for collisions between an object rectangle and a boundary rectangle returning a collision vector.
 * 
 * @param object_rect The rectangle of the object to check for collision.
 * @param boundary_rect The boundary rectangle to check against.
 * @param velocity The velocity vector of the object.
 * @return Vector2 A vector indicating collision on both x and y (-1 for collision, 1 otherwise).
 */
Vector2 CheckCollisionBoxPro(Rectangle object_rect, Rectangle boundary_rect, Vector2 velocity) {

    Vector2 collision_vector = {1.0f, 1.0f};  // Default: no collision (velocity remains 1)

    // Check if any corner of the object rectangle is within the boundary rectangle
    if (CheckBoundaryContainment((Vector2){object_rect.x, object_rect.y}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x + object_rect.width, object_rect.y}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x, object_rect.y + object_rect.height}, boundary_rect) ||
        CheckBoundaryContainment((Vector2){object_rect.x + object_rect.width, object_rect.y + object_rect.height}, boundary_rect)) {

        // Reverse horizontal velocity if collision occurs on the x-axis
        if ((object_rect.x <= boundary_rect.x && velocity.x > 0) || 
            (object_rect.x + object_rect.width >= boundary_rect.x + boundary_rect.width && velocity.x < 0)) {
            collision_vector.x *= -1.0f;
        } 

        // Reverse vertical velocity if collision occurs on the y-axis
        if ((object_rect.y <= boundary_rect.y && velocity.y > 0) || 
            (object_rect.y + object_rect.height >= boundary_rect.y + boundary_rect.height && velocity.y < 0)) {
            collision_vector.y *= -1.0f;
        }
    }

    return collision_vector;  // Return -1 for any axis with collision, 1 otherwise
}

/**
 * @brief Generate an encoded string representation of a value within a range.
 * 
 * @param value The current value to encode.
 * @param minValue The minimum value of the range.
 * @param maxValue The maximum value of the range.
 * @param numLines The number of lines in the encoded string.
 * @return std::string The encoded string representation.
 */
std::string generateEncodedString(float value, float minValue, float maxValue, int numLines) {
    float percent = (value - minValue) / (maxValue - minValue); // Normalize value to [0, 1]
    percent = std::fmax(0.0f, std::fmin(1.0f, percent)); // Clamp percent to [0, 1]
    int fillPosition = std::min((int)(percent * numLines), numLines - 1); // Determine fill position

    std::string filler = "."; // Default filler character
    std::string encoding = ""; // Encoded string

    for (int j = numLines - 1; j >= 0; j--) { // Build encoded string from top to bottom
        encoding += (j == fillPosition) ? "#" : "."; // Add '#' at fill position, '.' otherwise
    }

    return encoding; // Return the encoded string
}

/**
 * @brief Prints the encoded string to the console, overwriting the previous output to simulate a moving visual.
 * 
 * @param encoding The encoded string to render.
 * @param lineLength The number of characters per line.
 * @param numLines The total number of lines to render.
 */
void renderEncodedString(std::string &encoding, int lineLength, int numLines) {
    // Move cursor up numLines to overwrite previous output
    for (int i = 0; i < numLines; i++) {
        std::cout << "\033[A" << std::flush;  // ANSI escape code to move cursor up
    }

    // Render the encoded string line by line
    for (int i = 0 ; i < numLines; i++) { 
        for(int j = lineLength-1; j >= 0; j--){ // Iterate through columns in reverse
            if(i + (j * numLines) >= encoding.size()){ // Check bounds of encoding string
                break;
            }
            std::cout << encoding[i + (j * numLines)]; // Print character at calculated position
        }
        std::cout << std::endl; // Move to the next line after rendering
    }
}

#endif // UTILS_CPP