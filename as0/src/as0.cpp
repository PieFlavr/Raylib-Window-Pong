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


// TEXT BOX/FONT SETTINGs
#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600

#define INITIAL_FONT_SIZE 50
#define INITIAL_FONT_SPACING 2

#define TEXT_VELOCITY 10

#define MAX_HUE 360 // COLOR/RGB SETTINGs
#define BOUNCE_HUE_OFFSET 100
#define DEFAULT_HUE_OFFSET 3

#define DEFAULT_FPS 30

int main(){
    Vector2 textPos, textVelocity, textSize; //Text Properties
    Vector3 textColorHSV (0,1.0,1.0); //Default Text Color
    float textVelocityAngle;

    std::string displayText = "Hello World!";
    bool enableAnimation = false;

    raylib::Window window(INITIAL_WIDTH, INITIAL_HEIGHT, "CS381 - Assignment 0"); //Feature #1 - Window Title (5 pts)
    raylib::Text text(GetFontDefault(), displayText.c_str(), INITIAL_FONT_SIZE, INITIAL_FONT_SPACING,WHITE);
    SetTargetFPS(DEFAULT_FPS); //For preventing too much resource usage, artifacting, and resizing issues.

    window.SetState(FLAG_WINDOW_RESIZABLE); //Feature #2 - Resizable Window (5 pts)

    while(!window.ShouldClose()){ //Keeps window open until closed by user. 
        window.BeginDrawing();

        ClearBackground(BLACK);

        textSize.x = text.MeasureEx().x; //Updates text box sizes
        textSize.y = text.MeasureEx().y;

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ //Start/reset animation on click
            enableAnimation = !enableAnimation;

            textVelocityAngle = ((float)rand() / (RAND_MAX)) * 2.0 * PI; //Random direction per reset.

            //Calculates equivalent cartesian velocities.
            textVelocity.x = (std::cos(textVelocityAngle)) * TEXT_VELOCITY; //Could have done more general vector/matrix magic... 
            textVelocity.y = (std::sin(textVelocityAngle)) * TEXT_VELOCITY; //...but too annoying and complicated for first assignment.
        }

        //E.C. Feature #1 - Animation (+10 pts)
        if(enableAnimation){ //Basic logo bounce code, changes color on bounce and slight angle offset each bounce for fun.
            Vector2 textTargetPos = textPos;

            textPos.x += textVelocity.x; //Predict Next Text Location
            textPos.y += textVelocity.y;

            //Check if out of bounds, calculates text box boundaries
            bool XhitboxCheckMax = (textPos.x+textSize.x) >= window.GetWidth();
            bool XhitboxCheckMin = textPos.x <= 0;
            bool YhitboxCheckMax = (textPos.y+textSize.y) >= window.GetHeight();
            bool YhitboxCheckMin = textPos.y <= 0;

            if(XhitboxCheckMax || XhitboxCheckMin || YhitboxCheckMax || YhitboxCheckMin){ //Simple Collision Check
                if(XhitboxCheckMax || XhitboxCheckMin){ //Reflect X Velocity + Hue Offset
                    if(XhitboxCheckMax){ //Bounds x-position properly inside the window
                        textPos.x = window.GetWidth()-(1+textSize.x);
                    } else if(XhitboxCheckMin){
                        textPos.x = 1;
                    }
                    textVelocity.x *= -1;
                    textColorHSV.x = ((int)(textColorHSV.x+BOUNCE_HUE_OFFSET))%MAX_HUE; 
                }
                if(YhitboxCheckMax || YhitboxCheckMin){ //Reflect Y Velocity + Hue Offset
                    if(YhitboxCheckMax){ //Bounds y-position properly inside the window
                        textPos.y = window.GetHeight()-(1+textSize.y);
                    } else if(YhitboxCheckMin){
                        textPos.y = 1;
                    }
                    textVelocity.y *= -1;
                    textColorHSV.x = ((int)(textColorHSV.x+BOUNCE_HUE_OFFSET))%MAX_HUE;
                }
            }
            std::cout << textPos.x << " " << textPos.y << std::endl;
            //textPos.x += textVelocity.x; //Move Text
            //textPos.y += textVelocity.y;
        } else {
            //Feature #4 - Text Centering (15 pts)
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