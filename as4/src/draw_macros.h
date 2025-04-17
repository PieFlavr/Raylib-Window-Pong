#ifndef DRAW_MACROS_H
#define DRAW_MACROS_H

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

#define GET_WINDOW_INFO\   
        Vector2 window_pos = {GetWindowPosition().x, GetWindowPosition().y};\
        Vector2 window_dim = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};\
        Vector2 window_center = Vector2Add(window_pos,Vector2Multiply(window_dim, {0.5, 0.5}));\
        Vector2 window_offset = Vector2Subtract(window_center, screen_center);\

#define GET_NORMALIZED_WINDOW_INFO\ 
        Vector2 normalized_position = Vector2Divide(window_offset, screen_center);\
        float square_y_normal = window_offset.y / screen_center.x;\
        normalized_position.y *= -1;\
\
        Vector3 world_offset = {normalized_position.x * diameter_width, normalized_position.y * diameter_width, 0};\
        float reduction_factor = 1.0f / (1.0f + Vector2Length({normalized_position.x, square_y_normal}));\

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

#define RESET_CAMERA\ 
    camera.fovy = camera_base_fov;\
    camera.target = camera_base_target;\
    camera.position = camera_base_position;\

#define DRAW_VIEWPORT_VIEW\
        float rectX = (window_pos.x/ screen_dim.x) * GetScreenWidth();\
        float rectY = (window_pos.y/ screen_dim.y) * GetScreenHeight();\
        float rectWidth = window_dim.x / screen_dim.x * GetScreenWidth();\
        float rectHeight = window_dim.y / screen_dim.y * GetScreenHeight();\
\
        DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, RED);\

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