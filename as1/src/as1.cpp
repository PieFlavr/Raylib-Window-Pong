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
#include "VolumeControl.h"

#include <iostream>
#include <string>
#include <cmath>

#define DEFAULT_FPS 60

raylib::Music ping;

void PingButton(){
 // ijiji
}

int main(){
    raylib::Window window(300,350,"Hello World");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    raylib::AudioDevice audio;

    SetTargetFPS(DEFAULT_FPS); //For preventing too much resource usage, artifacting, and resizing issues.

    raylib::Text text;
    text.SetText("Hello World");
    text.SetFontSize(30);

    auto guiState = InitGuiVolumeControl();
    
    ping.Load("../../assets/audio/ping.wav");
    ping.SetLooping(false);
    ping.Play();

    while(!window.ShouldClose()){

        window.BeginDrawing();

        ClearBackground(BLACK);
        GuiVolumeControl(&guiState);
        ping.Update();
        
        window.EndDrawing(); 

    }
}