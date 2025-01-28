/**
 * @file as0.cpp
 * @author Lucas Pinto
 * @brief 
 * @version 0.1
 * @date 2025-01-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <string>

#include "raylib-cpp.hpp"

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600
#define INITIAL_FONT_SIZE 20
#define INITIAL_FONT_SPACING 2

#define DEFAULT_FPS 30

/**
 * @brief 
 * 
 * @return int 
 */
int main(){
    Vector2 windowSize, textPos; 
    char* displayText = "Hellow World!";

    raylib::Window window(INITIAL_WIDTH, INITIAL_HEIGHT, "CS381 - Assignment 0"); //Feature #1 - Window Title (5 pts)
    SetTargetFPS(DEFAULT_FPS); //For preventing too much resource usage, artifacting, and resizing issues.

    window.SetState(FLAG_WINDOW_RESIZABLE); //Feature #2 - Resizable Window (5 pts)

    while(!window.ShouldClose()){ //Keeps window open until closed by user. 
        window.BeginDrawing();

        ClearBackground(BLACK); 

        windowSize.x = window.GetWidth();
        windowSize.y = window.GetHeight();

        //Feature #4 - Text Centering (15 pts)
        textPos.x = (windowSize.x/2) - (MeasureTextEx(GetFontDefault(), displayText, INITIAL_FONT_SIZE,INITIAL_FONT_SPACING).x/2);
        textPos.y = (windowSize.y/2) - (MeasureTextEx(GetFontDefault(), displayText, INITIAL_FONT_SIZE,INITIAL_FONT_SPACING).y/2);
        
        DrawTextEx(GetFontDefault(),displayText,textPos, INITIAL_FONT_SIZE,INITIAL_FONT_SPACING,WHITE); //Feature #3 - Text Display (15 pts)
        
        window.EndDrawing();
    }

    return 0;
}