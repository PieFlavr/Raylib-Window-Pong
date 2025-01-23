#include "BufferedRaylib.hpp"
#include "raymath.h"

#include <iostream>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Buffered Raylib Example");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------



    // The buffered input manages all of the actions
    raylib::BufferedInput input;

    // Create a movement action which creates a vector pointing in a direction based on which combination of the wasd key are pressed
    input.actions["move"] = raylib::Action::wasd()
        .SetCallback([&ballPosition](Vector2 dir, Vector2 delta) {
            // Whenever the set of pressed keys changes, move the ball based on the pressed set
            ballPosition = Vector2Add(Vector2Scale(Vector2Normalize(dir), -10), ballPosition);
            std::cout << "{" << dir.x << ", " << dir.y << "} - {" << delta.x << ", " << delta.y << "}" << std::endl;
        }).move();

    // Create a mouse action which creates a vector representing the position of the mouse
    input.actions["mouse"] = raylib::Action::mouse_position().SetCallback([](Vector2 pos, Vector2 delta){
        // Calculate the relative mouse movement using the base raylib function
        auto rlDelta = GetMouseDelta();
        // Print the mouse's position and its relative movement using both calculation methods (second and third pairs of numbers should be the same)
        std::cout << "{" << pos.x << ", " << pos.y << "} - {" << delta.x << ", " << delta.y << "} - {" << rlDelta.x << ", " << rlDelta.y << "}" << std::endl;
    }).move();

    // An example showing a longer form method of creating an action
    {
        // Create the action (in this case one which listens for the left mouse button)
        auto a = raylib::Action::mouse_button(MOUSE_BUTTON_LEFT);
        // Add a callback to be invoked when the mouse button is pressed (multiple callbacks can be added to a single action)
        // (the same callback can also listen to multiple actions in which case the name of the action passed as the first argument to the callback becomes important)
        a.AddCallback([](uint8_t pressed, bool wasPressed){
            // If this is a press and not a release, print a message!
            if(!wasPressed) std::cout << "Bang!" << std::endl;
        });
        // Add the action to a BufferedInput so that it can be automatically pumped
        input.actions["shoot"] = std::move(a); // NOTE: Actions are move only!
    }
    // It is also possible to extract the last state (as calculated last time BufferedInput.PollEvents was called)
    std::cout << (bool)input.actions["shoot"].data.button.last_state << std::endl;

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        // Processing and callback invocation occur whenever messages are pumped
        input.PollEvents();

        // NOTE: Be careful to access the correct union alternative when extracting the last state
        std::cout << (bool)input.actions["shoot"].data.button.last_state << std::endl;


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(ballPosition, 50, MAROON);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}