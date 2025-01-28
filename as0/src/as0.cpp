/**
 * @file as0.cpp
 * @author Lucas Pinto
 * @brief Code submission for Assignment 0 consisting of a resizable window of requested title, (initially) centered text, 
 *        and a toggleable "TV Logo Bounce" animation with rainbow colors.
 * @version 0.1
 * @date 2025-01-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "raylib-cpp.hpp"

#include <iostream>
#include <string>
#include <cmath>

// TEXT SETTINGs
#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600

#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING 2

#define MAX_HUE 360
#define BOUNCE_HUE_OFFSET 100
#define DEFAULT_HUE_OFFSET 3

#define TEXT_VELOCITY 10

#define DEFAULT_FPS 30

int main(){
    Vector2 textPos, textVelocity, textSize; //Text Properties
    Vector3 textColorHSV (0,1.0,1.0); //Default Text Color
    float textVelocityAngle;

    char* displayText = "Hello World!";
    bool enableAnimation = false;

    raylib::Window window(INITIAL_WIDTH, INITIAL_HEIGHT, "CS381 - Assignment 0"); //Feature #1 - Window Title (5 pts)
    raylib::Text text(GetFontDefault(),displayText, INITIAL_FONT_SIZE, INITIAL_FONT_SPACING,WHITE);
    SetTargetFPS(DEFAULT_FPS); //For preventing too much resource usage, artifacting, and resizing issues.

    window.SetState(FLAG_WINDOW_RESIZABLE); //Feature #2 - Resizable Window (5 pts)

    while(!window.ShouldClose()){ //Keeps window open until closed by user. 
        window.BeginDrawing();

        ClearBackground(BLACK);

        textSize.x = text.MeasureEx().x; //Updates text box sizes
        textSize.y = text.MeasureEx().y;

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ // Toggles animations on click
            enableAnimation = !enableAnimation;

            textVelocity.x = (std::cos(textVelocityAngle)) * TEXT_VELOCITY;
            textVelocity.y = (std::sin(textVelocityAngle)) * TEXT_VELOCITY;
        }

        //E.C. Feature #1 - Animation (+10 pts)
        if(enableAnimation){ //Basic logo bounce code, changes color on bounce and slight angle offset each bounce for fun.
            Vector2 textTargetPos = textPos;

            textTargetPos.x += textVelocity.x; //Predict Next Text Location
            textTargetPos.y += textVelocity.y;

            //Check if out of bounds, calculates text box boundaries
            bool XhitboxCheck = ((textTargetPos.x+textSize.x) >= window.GetWidth() || (textTargetPos.x) <= 0);
            bool YhitboxCheck = ((textTargetPos.y+textSize.y) >= window.GetHeight() || (textTargetPos.y) <= 0);

            if(XhitboxCheck || YhitboxCheck){ //Simple Collision Check
                if(XhitboxCheck){ //Reflect X Velocity + Hue Offset
                    textVelocity.x *= -1;
                    textColorHSV.x = ((int)(textColorHSV.x+BOUNCE_HUE_OFFSET))%MAX_HUE; 
                }
                if(YhitboxCheck){ //Reflect Y Velocity + Hue Offset
                    textVelocity.y *= -1;
                    textColorHSV.x = ((int)(textColorHSV.x+BOUNCE_HUE_OFFSET))%MAX_HUE;
                }
            }

            textPos.x += textVelocity.x; //Move Text
            textPos.y += textVelocity.y;
        } else {
            //Feature #4 - Text Centering (15 pts)
            textVelocityAngle = ((float)rand() / (RAND_MAX)) * 2.0 * PI;

            textPos.x = (window.GetWidth()/2) - (textSize.x/2);
            textPos.y = (window.GetHeight()/2) - (textSize.y/2);
        }

        textColorHSV.x = ((int)(textColorHSV.x+DEFAULT_HUE_OFFSET))%MAX_HUE; //E.C. Feature #1 - Multiple Colors (+10 pts)
        // ^^^ performs rainbow lighting on the main text
        text.SetColor(ColorFromHSV(textColorHSV.x,textColorHSV.y,textColorHSV.z));

        //Instructions draw 
        DrawTextEx(GetFontDefault(), "[Left Click] to Start/Reset Animation", Vector2(10,10), INITIAL_FONT_SIZE/2, INITIAL_FONT_SPACING, WHITE);
        text.Draw(textPos); //Feature #3 - Main Text Display (15 pts)
        
        window.EndDrawing();
    }

    return 0;
}