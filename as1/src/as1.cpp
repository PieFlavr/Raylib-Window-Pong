/**
 * @file as1.cpp
 * @author Lucas Pinto
 * @brief Volume control demonstration using raylib-cpp and raylib that includes three
 *        sliders, one each for a Music, Dialouge, and Sound audio. There is a Ping button
 *        that plays a sound effect, and diagetic crowd audio along with music provided
 *        for the assignment that plays constantly. 
 * 
 *        All audio values are defaulted at 0 to prevent your ears getting blown off :)
 *        Can be changed if need be via DEFAULT_VOLUME macro (mostly a vestigial from testing)
 * @version 0.1
 * @date 2025-02-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "raylib-cpp.hpp"
#include "VolumeControl.h" 


#define DEFAULT_VOLUME 0
#define LOGIC_FPS 120
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
    double logicDelta = 1.0 / LOGIC_FPS;

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

    dialogue.SetLooping(true); //Feature #5 - Continuous Dialoge/Music (15 pts)
    music.SetLooping(true); 

    dialogue.SetVolume(DEFAULT_VOLUME); //Ensures audio is set to default volumes
    music.SetVolume(DEFAULT_VOLUME);
    ping.SetVolume(DEFAULT_VOLUME);

    dialogue.Play();
    music.Play();

    auto volumeControlGUI = InitGuiVolumeControl();

    volumeControlGUI.DialogueSliderValue = DEFAULT_VOLUME*100;
    volumeControlGUI.MusicSliderValue = DEFAULT_VOLUME*100;
    volumeControlGUI.SFXSliderValue = DEFAULT_VOLUME*100;

    while(!window.ShouldClose()){

        // Update Block
        double currentTime = GetTime();
        
        dialogue.Update();
        music.Update();

        // Logic Block
        if(currentTime - lastDrawTime >= logicDelta){ 
            dialogue.SetVolume(volumeControlGUI.DialogueSliderValue/100.0); //Feature #6 - Connect Sliders to Audio
            music.SetVolume(volumeControlGUI.MusicSliderValue/100.0);
            ping.SetVolume(volumeControlGUI.SFXSliderValue/100.0);
        }

        // Rendering Block
        if(currentTime - lastDrawTime >= drawDelta){
            window.BeginDrawing();

            ClearBackground(BLACK);
            GuiVolumeControl(&volumeControlGUI); //Feature #2 - Load the Volume Control GUI
            
            window.EndDrawing(); 
        }
        

    }
}