#include "raylib-cpp.hpp"
#include "VolumeControl.h" 

#include <iostream>
#include <string>
#include <cmath>

#define DRAW_FPS 60

raylib::Sound ping; //Using raylib::Sound since shorter sound bites (loaded into memory apparently)
raylib::Music dialogue, music; //Using raylib::Music since plays for longer (disk streaming apparently)
                                //Judgement says to put dialogue as music since it loops forever anyway.
void PingButton(){
    ping.Play(); //Feature #4 -Sound Effect on Button Press (10 points)
}

int main(){

    double lastDrawTime = 0;
    double drawDelta = 1.0 / DRAW_FPS; 

    raylib::Window window(300,350,"CS381 - Assignment 1"); //Feature #1 - Window Title (1 pts)
    window.SetState(FLAG_WINDOW_RESIZABLE);

    raylib::AudioDevice defaultDevice;

    //Feature #3 - Load 3 Audio Files (30 pts)
    music.Load("../../assets/audio/price-of-freedom.mp3");  //Sound Effect (10 pts)
    dialogue.Load("../../assets/audio/crowd.wav"); //Dialogue (10 pts)
    ping.Load("../../assets/audio/ping.wav");  //Music (10pts)
    /*
        AGHHHH THIS HURT MY BRAIN
        Was looking at the raylib documentation, forgetting we're using the raylib-cpp wrapper.
        Apparently raylib handles loading into Sound different than raylib-cpp.
        
        That, and the utter confusion between using raylib vs. raylib-cpp namespaces
        hurt my brain for a good 30 minutes.
        
        I see how it works now, with OG raylib stuff accessible w/o name specifies
        and raylib-cpp stuff accessible via raylib:: namespace.
     */

    dialogue.SetLooping(false);
    music.SetLooping(false);

    dialogue.SetVolume(0.5); //Set each to default volumes
    music.SetVolume(0.5);
    ping.SetVolume(0.5);

    dialogue.Play();
    music.Play();

    auto guiState = InitGuiVolumeControl();

    while(!window.ShouldClose()){

        double currentTime = GetTime();
        
        dialogue.Update();
        music.Update();

        // Rendering Block
        if(currentTime - lastDrawTime >= drawDelta){
            window.BeginDrawing();

            ClearBackground(BLACK);
            GuiVolumeControl(&guiState); //Feature #2 - Load the Volume Control GUI
            
            window.EndDrawing(); 
        }
        

    }
}