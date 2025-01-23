# BufferedRaylib

```cpp
// Create a BufferedInput object named 'input' for handling user input
raylib::BufferedInput input;

// Create an action named "move" and configure it for the WASD keys
input.actions["move"] = raylib::Action::wasd()
    .set_callback([&ballPosition](const std::string_view name, Vector2 dir, Vector2 delta) {
        // Update the 'ballPosition' based on the input direction
        ballPosition = Vector2Add(Vector2Scale(Vector2Normalize(dir), -10), ballPosition);
        // Print the direction and delta values to the console
        std::cout << "{" << dir.x << ", " << dir.y << "} - {" << delta.x << ", " << delta.y << "}" << std::endl;
    }).make_unique(); // Actions uses unique pointers to its actions... make_unique conveniently erases the type

// Create an action named "mouse" to track mouse position
input.actions["mouse"] = raylib::Action::mouse_position().set_callback([](const std::string_view name, Vector2 pos, Vector2 delta){
    // Get the mouse delta using Raylib's GetMouseDelta() function
    auto rlDelta = GetMouseDelta();
    // Print mouse position, delta, and Raylib delta (which should be the same as delta) to the console
    std::cout << "{" << pos.x << ", " << pos.y << "} - {" << delta.x << ", " << delta.y << "} == {" << rlDelta.x << ", " << rlDelta.y << "}" << std::endl;
}).make_unique();

// Create an action named "shoot" for the left mouse button
{
    auto a = raylib::Action::mouse_button(MOUSE_BUTTON_LEFT);
    // Add a callback function to execute when the left mouse button is pressed
    a.callback += [](const std::string_view, uint8_t pressed, bool wasPressed){
        if(!wasPressed) std::cout << "Bang!" << std::endl;
    };
    // Assign the action to the "shoot" input action
    input.actions["shoot"] = a.make_unique();
    // NOTE: "a" has been moved into actions["shoot"] and is no longer valid here, accessing it is now UNDEFINED BEHAVIOR!
}

// Print the last state of the "shoot" action as a boolean value to the console
std::cout << (bool)input.actions["shoot"]->as<raylib::ButtonAction>().last_state << std::endl;
```