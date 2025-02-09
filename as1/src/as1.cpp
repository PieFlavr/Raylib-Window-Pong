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
 * 
 *       (2/9) UDPATE: All Extra Credit (EC) features have effectively been implemented, 
 *                     you can easily search for each via [CTRL+F] and looking for "EC Feature"
 * @version 0.1
 * @date 2025-02-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "raylib-cpp.hpp"
#include "VolumeControl.h" 

#define DEFAULT_VOLUME 0
#define LOGIC_FPS 60
#define DRAW_FPS 60

#define HIGHLIGHT_BRIM -8
#define VOLUME_KEYS_INCREMENT 0.1

Color backgroundColor = WHITE;
bool themeToggleState = 0; //For handling menu theme logic. (0 = "light"; 1 = "dark")
int focusedElement = -1; //Only focuses once [TAB] is pressed.

raylib::Sound ping; //Using raylib::Sound since shorter sound bites (loaded into memory apparently)
raylib::Music dialogue, music; //Using raylib::Music since plays for longer (disk streaming apparently)
                                //Judgement says to put dialogue as music since it loops forever anyway.
                            
void PingButton();
void ModeToggleButton();
void GuiVolumeControlPlus(GuiVolumeControlState *state);

int main(){

    double lastDrawTime = 0;
    double drawDelta = 1.0 / DRAW_FPS; 
    double logicDelta = 1.0 / LOGIC_FPS;

    raylib::Window window(300,400,"CS381 - Assignment 1"); //Feature #1 - Window Title (1 pts)
    window.ClearState(FLAG_WINDOW_RESIZABLE); //Theoretically ensures the window can't be resizable... 
    // ^^^ can't tell if the above works, nothing happens on WSL for some reason? Needs further testing for anything definitive.

    raylib::AudioDevice defaultDevice; 

    //Feature #3 - Load 3 Audio Files (30 pts) + EC Feature #2 - Custom Audio Off the Internet
    music.Load("../../assets/audio/musicEC.mp3");  //Sound Effect (10 pts)
    dialogue.Load("../../assets/audio/dialogueEC.mp3"); //Dialogue (10 pts)
    ping.Load("../../assets/audio/pingEC.mp3");  //Music (10pts)
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

            // Controls Block
            if(IsKeyPressed(KEY_TAB)){ // EC Feature #4 - Slider Tab Switch (5 points)
                focusedElement = (focusedElement+1)%3;
            }

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                focusedElement = -1; //Deselects keyboard focus
            }

            if(IsKeyDown(KEY_LEFT)){    // EC Feature #3 - Arrow key Controls (5 points)
                                        // Had to actually drop LOGIC_FPS to 60 for this :p
                switch(focusedElement){ //Uses fmax() + fmin() to bound so no volume bound issues.
                    case 0:             //... could be abstracted in a function but I want to sleep right now 
                        volumeControlGUI.SFXSliderValue = fmax(volumeControlGUI.SFXSliderValue - VOLUME_KEYS_INCREMENT,0);
                        break;
                    case 1: 
                        volumeControlGUI.MusicSliderValue = fmax(volumeControlGUI.MusicSliderValue - VOLUME_KEYS_INCREMENT,0);
                        break;
                    case 2:
                        volumeControlGUI.DialogueSliderValue = fmax(volumeControlGUI.DialogueSliderValue - VOLUME_KEYS_INCREMENT,0);
                        break;
                    default:
                        break;
                }
            } else if(IsKeyDown(KEY_RIGHT)){
                switch(focusedElement){
                    case 0:
                        volumeControlGUI.SFXSliderValue = fmin(volumeControlGUI.SFXSliderValue + VOLUME_KEYS_INCREMENT,100);
                        break;
                    case 1: 
                        volumeControlGUI.MusicSliderValue = fmin(volumeControlGUI.MusicSliderValue + VOLUME_KEYS_INCREMENT,100);
                        break;
                    case 2:
                        volumeControlGUI.DialogueSliderValue = fmin(volumeControlGUI.DialogueSliderValue + VOLUME_KEYS_INCREMENT,100);
                        break;
                    default:
                        break;
                }
            }
        }

        // Rendering Block
        if(currentTime - lastDrawTime >= drawDelta){
            window.BeginDrawing();

            ClearBackground(backgroundColor);
            GuiVolumeControlPlus(&volumeControlGUI); //Feature #2 - Load the Volume Control GUI
            
            window.EndDrawing(); 
        }
        

    }
}


/**
 * @brief Run when the "ping button" is pressed on the GUI. Plays the "ping" sound.
 * 
 */
void PingButton(){ //Feature #4 - Sound Effect on Button Press (10 points)
    ping.Play(); 
 }
 
 /**
  * @brief Effectively toggles between a "light" and "dark" theme.
  * 
  */
 void ModeToggleButton(){ 
     if(themeToggleState){
        backgroundColor = WHITE;
        GuiLoadStyleDefault();
     } else {
        backgroundColor = BLACK; // EC Feature #1 - Dark Mode Option (5 points)
        // EC Feature #5 - Dark Theme Option (5 points) 
        // Is this cheating? I'm not sure if we're supposed to "make" a dark theme...
        // ...or if we're supposed to MAKE a dark theme, slap it in assets, and use it that way?
        // welp, either way it works so ¯\_(ツ)_/¯
        GuiLoadStyle("../../raylib-cpp/raygui/styles/dark/dark.rgs");
     }
     themeToggleState = !themeToggleState;
 }
 
 /**
  * @brief Custom re-implementation of the GuiVolumeControl() function to include "dark-mode" button.
  *        mostly identical with exception to a few size differences, additions, and color customization.
  *          
  *        NOTE: I would normally just edit "VolumeControl.h" directly but I wasn't sure if we were allowed that.
  *              I would also have implemented this in "implementations.cpp", but from what I can glean that is more
  *              of a pseudo-interface to enforce implementations of functions used in functions. However, I am unsure
  *              how much of either I can edit, so I'm just going to do this. 
  * 
  *             This is really bad practice, but that's that and this is this so whatever.
  * @param state The "state" struct from "VolumeControl.h" tracking all of the slider variables.
  */
 void GuiVolumeControlPlus(GuiVolumeControlState *state) {
     // Const text
     const char *VolumeGroupText = "Volume Controls";    // GROUPBOX: VolumeGroup
     const char *SFXGroupText = "SFXVolume";    // GROUPBOX: SFXGroup
     const char *SFXSliderText = "";    // SLIDER: SFXSlider
     const char *MusicGroupText = "MusicVolume";    // GROUPBOX: MusicGroup
     const char *MusicSliderText = "";    // SLIDER: MusicSlider
     const char *DialogueGroupText = "DialogueVolume";    // GROUPBOX: DialogueGroup
     const char *DialogueSliderText = "";    // SLIDER: DialogueSlider
     const char *PingButtonText = "Ping";    // BUTTON: PingButton
 
     const char *ThemeButtonText = "Theme Toggle";    // BUTTON: Theme Toggle
     
     // Draw controls
     GuiGroupBox((Rectangle){ state->anchor01.x + 0, state->anchor01.y + 0, 256, 264 }, VolumeGroupText);
 
     //Sounds Group
     GuiGroupBox((Rectangle){ state->anchor01.x + 24, state->anchor01.y + 24, 208, 56 }, SFXGroupText);
     GuiLabel((Rectangle){ 64, 64, 120, 24 }, TextFormat("%.0f%%", state->SFXSliderValue));
     if(focusedElement == 0){ //Focus Highlight 
         GuiGroupBox((Rectangle){ state->anchor01.x + 24 - HIGHLIGHT_BRIM, 
                                            state->anchor01.y + 24 - HIGHLIGHT_BRIM, 
                                            208+(HIGHLIGHT_BRIM*2), 
                                            56+(HIGHLIGHT_BRIM*2)}, 
                                            NULL);
     }
     state->SFXSliderValue = GuiSlider((Rectangle){ state->anchor01.x + 72, state->anchor01.y + 40, 144, 24 }, SFXSliderText, NULL, state->SFXSliderValue, 0, 100);
 
     //Music Group
     GuiGroupBox((Rectangle){ state->anchor01.x + 24, state->anchor01.y + 104, 208, 56 }, MusicGroupText);
     GuiLabel((Rectangle){ 64, 144, 120, 24 }, TextFormat("%.0f%%", state->MusicSliderValue));
     if(focusedElement == 1){ //Focus Highlight
         GuiGroupBox((Rectangle){ state->anchor01.x + 24 - HIGHLIGHT_BRIM, 
                                            state->anchor01.y + 104 - HIGHLIGHT_BRIM, 
                                            208+(HIGHLIGHT_BRIM*2), 56+(HIGHLIGHT_BRIM*2)}, 
                                            NULL);
     }
     state->MusicSliderValue = GuiSlider((Rectangle){ state->anchor01.x + 72, state->anchor01.y + 120, 144, 24 }, MusicSliderText, NULL, state->MusicSliderValue, 0, 100);
 
     //Dialouge Group
     GuiGroupBox((Rectangle){ state->anchor01.x + 24, state->anchor01.y + 184, 208, 56 }, DialogueGroupText);
     GuiLabel((Rectangle){ 64, 224, 120, 24 }, TextFormat("%.0f%%", state->DialogueSliderValue));
     if(focusedElement == 2){ //Focus Highlight
         GuiGroupBox((Rectangle){ state->anchor01.x + 24 - HIGHLIGHT_BRIM, 
                                            state->anchor01.y + 184 - HIGHLIGHT_BRIM, 
                                            208+(HIGHLIGHT_BRIM*2), 
                                            56+(HIGHLIGHT_BRIM*2)}, 
                                            NULL);
     }
     state->DialogueSliderValue = GuiSlider((Rectangle){ state->anchor01.x + 72, state->anchor01.y + 200, 144, 24 }, DialogueSliderText, NULL, state->DialogueSliderValue, 0, 100);
     
     if (GuiButton((Rectangle){ 24, 304, 256, 24 }, PingButtonText)) PingButton();
     if (GuiButton((Rectangle){ 24, 304+24+10, 256, 24 }, ThemeButtonText)) ModeToggleButton();
 }