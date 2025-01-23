#include <iostream>

#include "raylib-cpp.hpp"

int main(){

    raylib::Window window(800, 400, "Hello World");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    std::cout << "Hello World" << std::endl;

    while(!window.ShouldClose()){
        window.BeginDrawing();
        window.EndDrawing();
    }

    return 0;
}