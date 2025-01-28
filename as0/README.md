# QUIZ QUESTIONs and ANSWERs
**(1A) Include a 2-3 sentence explanation of how to make a window resizable (5 points)...**

The window is kept resizable while preserving backgrounds and text through the following lines...
```c++
raylib::Window window(INITIAL_WIDTH, INITIAL_HEIGHT, "CS381 - Assignment 0"); //Feature #1 - Window Title (5 pts)
//*... other code ...*//
window.SetState(FLAG_WINDOW_RESIZABLE); //Feature #2 - Resizable Window (5 pts)
while(!window.ShouldClose()){ //Keeps window open until closed by user. 
    window.BeginDrawing();
    ClearBackground(BLACK);
    //*... other code ...*//
    window.EndDrawing();
}
```
The window is created with the requested decoration via the `window()` constructor. Then the window's resizable flag is set through the `SetState()` mutator with the pre-defined `FLAG_WINDOW_RESIZABLE` enum. Until closed by the user, the window is constantly re-drawn with the background cleared via `ClearBackground(BLACK)` each frame allowing for a proper resizing by the user without visual artifacts.

**(1B) ...And a 2-3 sentence explanation of how to keep text centered in a resizing window (5 points).**

While the animation is not enabled, the text is centered even during resizing via re-drawing the frame with its new calculated center.
The center is calculated using the following lines...
```c++
textSize.x = text.MeasureEx().x; //Updates text box sizes
textSize.y = text.MeasureEx().y;
//*... other code ...*//
textPos.x = (window.GetWidth()/2) - (textSize.x/2);
textPos.y = (window.GetHeight()/2) - (textSize.y/2);
//*... other code ...*//
text.Draw(textPos); //Feature #3 - Main Text Display (15 pts)
```
In essence, the size of the text is measured each frame and stored in the Vector2 textSize, and is used along with the window's size
to calculate where the text box's position should be placed (origin at top left for both window and text box). This is then stored in
the Vector2 textPos. At the very end of the main loop, the text is drawn with this calculated position. However, if animation is enabled,
the Vector2 textPos is determined differently to allow it's bouncing animation.

**(2) Describe how to fetch your git submodules (5 points) with commands that can be copy and pasted into a Linux terminal (5 points).**

Ensure you have the following dependencies listed as follows, or simply run the following command...
```bash
sudo apt install cmake g++ libgl-dev libwayland-dev wayland-protocols libxrandr-dev pkg-config libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev mesa-utils build-essential cmake xorg-dev
```
Sometimes it may run normally without the need to manually fetch the submodules. 
Otherwise, to feth them manually, run the following...
```bash
git clone --recurse-submodules https://github.com/joshuadahlunr/raylib-cpp.git
```
If there are still issues with above, then instead run the following in order...
```bash
git submodule add https://github.com/joshuadahlunr/raylib-cpp.git
git submodule init
git submodule update --init --recursive
```

**(3) Describe how to compile your code (10 points) with copy and pasteable commands (10 points).**

Ensure you have the following dependencies listed as follows, or simply run the following command.
```bash
sudo apt install cmake g++ libgl-dev libwayland-dev wayland-protocols libxrandr-dev pkg-config libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev mesa-utils build-essential cmake xorg-dev
```
Then, in order from the main repository directory, simply run the following commands in the terminal in order.
```bash
cd as0/build   # Only run if not already in build directory
cmake ..   # Generates makefile + fixes dependencies
make   # Generates as0 executable
```
If running into a wayland-related error during `cmake`, go into `CMakeCache.txt` and set this setting to OFF `GLFW_BUILD_WAYLAND:BOOL=OFF`. 
This may or may not disable *window decorations*, but by default without the above modificiation they should be visible and enabled.
Then, simply run the `as0` executable via `./as0`

**(4) Describe how to run you’re code (10 points) and enumerate its controls [I predict "not applicable" will be the most common set of controls for this assignment.] (10 points).**

To run the code after compilation, simply run the `as0` executable from the `../as0/build/` directory via
```bash
cd as0/build    # Only run if not already in build directory
./as0   # Runs program executable
```
Once the window is up, the only control is a [Left Click] to start/reset the animation, which randomly sends out the colored text at a random angle. The controls are also listed in the window on the top right. The text should bounce around the window, even during and after resizing.
