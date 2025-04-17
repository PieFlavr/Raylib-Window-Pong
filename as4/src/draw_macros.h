/**
 * @file draw_macros.h
 * @author Lucas Pinto
 * @brief A file containing macros for drawing, game logic, and updating camera properties.
 *        Made solely for the purpose of taking advantage of specific object-life properties without specifically implementing them.
 *        Had to adhere to "everything is a variable" rule, and given the power of scope operators this is very strong!
 * @version 0.1
 * @date 2025-04-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DRAW_MACROS_H
#define DRAW_MACROS_H

/**
 * @brief Draws a 3D scene with a sphere and grid.
 * 
 */
#define DRAW_3D_SCENE\
    do {\
        BeginMode3D(camera);\
\
        DrawSphere({0, 0, 0}, 1.0f, RED);\
        DrawGrid(10, 1.0f);\
        /**DrawModel(cow, (Vector3){0,0,0}, 30.0f, BLACK);**/\
        /**DrawSphere(camera.target, 1.0f, BLUE);**/\
    EndMode3D();\
    } while(0)

/**
 * @brief Determines the position of the game elements relative to a given window to draw them correctly.
 * 
 */
#define GAME_DRAW_LOGIC(base_window, return_window)\
    do {\
        Vector2 left_paddle_relative_pos = GetRelativePosition(left_paddle_pos, base_window, return_window);\
        Vector2 right_paddle_relative_pos = GetRelativePosition(right_paddle_pos, base_window, return_window);\
        Vector2 ball_relative_pos = GetRelativePosition(ball_pos, base_window, return_window);\
\
        DrawRectangleLines(left_paddle_relative_pos.x, left_paddle_relative_pos.y, left_paddle_size.x, left_paddle_size.y, RED);\
        DrawRectangleLines(right_paddle_relative_pos.x, right_paddle_relative_pos.y, right_paddle_size.x, right_paddle_size.y, BLUE);\
        DrawCircleV(ball_relative_pos, ball_radius, GREEN);\
    } while (0)

/**
 * @brief Gets the window information such as position, dimensions, and offsets for "object-like" access in a global context.
 * 
 */
#define GET_WINDOW_INFO\   
        Vector2 window_pos = {GetWindowPosition().x, GetWindowPosition().y};\
        Vector2 window_dim = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};\
        Vector2 window_center = Vector2Add(window_pos,Vector2Multiply(window_dim, {0.5, 0.5}));\
        Vector2 window_offset = Vector2Subtract(window_center, screen_center);\

/**
 * @brief Gets the window information normalized to the screen center. Primarily for use in the "faux" viewport camera effect.
 * 
 */
#define GET_NORMALIZED_WINDOW_INFO\ 
        Vector2 normalized_position = Vector2Divide(window_offset, screen_center);\
        float square_y_normal = window_offset.y / screen_center.x;\
        normalized_position.y *= -1;\
\
        Vector3 world_offset = {normalized_position.x * diameter_width, normalized_position.y * diameter_width, 0};\
        float reduction_factor = 1.0f / (1.0f + Vector2Length({normalized_position.x, square_y_normal}));\

/**
 * @brief Updates the camera to the globally calculated properties, mainly for the mimicking a viewport for each window.
 *              There's not thorough support for multiple viewports in raylib to see the same object like a literaly window, so this is a good-enough workaround.
 *              Not perfect, but looks cool enough!
 * 
 */
#define UPDATE_CAMERA\
        camera.position = Vector3Add(camera_base_position, world_offset);\
\
        Vector3 camera_translation = Vector3Add(\
            Vector3Add(\
                Vector3Scale(right, world_offset.x),\
                Vector3Scale(up, world_offset.y)\
            ),\
            Vector3Scale(forward, world_offset.z)\
        );\
\
        camera.target = Vector3Add(camera_base_target, camera_translation);\
        camera.position = Vector3Add(camera_base_position, camera_translation);\
        camera.fovy = camera_base_fov * reduction_factor;\

/**
 * @brief Resets the camera to its default state. This is useful for resetting the camera after each window draw.
 * 
 */
#define RESET_CAMERA\ 
    camera.fovy = camera_base_fov;\
    camera.target = camera_base_target;\
    camera.position = camera_base_position;\

/**
 * @brief Draws the approximate "viewport" area of a particular window on the window for cool visualization purposes! 
 * 
 */
#define DRAW_VIEWPORT_VIEW\
        float rectX = (window_pos.x/ screen_dim.x) * GetScreenWidth();\
        float rectY = (window_pos.y/ screen_dim.y) * GetScreenHeight();\
        float rectWidth = window_dim.x / screen_dim.x * GetScreenWidth();\
        float rectHeight = window_dim.y / screen_dim.y * GetScreenHeight();\
\
        DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, RED);\

/**
 * @brief A macro primarily handling window gravity and velocity. It updates the window position and velocity based on the provided parameters.
 * 
 */
#define DO_WINDOW_GRAVITY(_window_pos, _window_velocity, _window_dim)\ 
        Vector2 window_pos = _window_pos;\
        Vector2 window_velocity = _window_velocity;\
        Vector2 old_window_velocity = _window_velocity;\
        Vector2 window_dim = _window_dim;\
\
        Vector2 window_collision = CheckCollisionBoundary({window_pos.x, window_pos.y, window_dim.x, window_dim.y}, \
            {0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, window_velocity);\
\
        window_pos.x += (window_velocity.x * logicDelta);\
        window_pos.y += (window_velocity.y * logicDelta);\
\
        window_velocity.x *= window_collision.x;\
        window_velocity.y *= window_collision.y;\
\
        window_velocity.y += gravity * logicDelta;\
        window_velocity = Vector2Multiply(window_velocity, {pow(drag, logicDelta), pow(drag, logicDelta)});\
\
        if(Vector2Length(window_velocity) < 5.0f && \
            (old_window_velocity.x/fabs(old_window_velocity.x) != window_velocity.x/fabs(window_velocity.x))){\
            window_velocity.x = 0.0f;\
            window_velocity.y = 0.0f;\
        }\
\
        if(window_velocity.x == 0.0f && window_velocity.y == 0.0f){\
            if(gravity > 0){\
                window_pos.y = lerp(window_pos.y, screenHeight - window_dim.y, 0.1f);\
            } else {\
                window_pos.y = lerp(window_pos.y, 0, 0.1f);\
            }\
        }\
\
        window_pos.x = std::fmax(0, std::fmin(static_cast<float>(screenWidth) - window_dim.x, window_pos.x));\
        window_pos.y = std::fmax(0, std::fmin(static_cast<float>(screenHeight) - window_dim.y, window_pos.y));\

#endif // DRAW_MACROS_H