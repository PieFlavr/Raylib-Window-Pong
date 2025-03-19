# as7 README

## PREFACE

This project is likely structured slightly differently than similar projects. The following are key important details:

* There is a deliberate pre-existing empty (with exception of .gitkeep) `build` folder in each assignment folder.
* All assets folders and submodules such as `raylib-cpp` exist in the main directory (there are no copies per assignment directory).
* The ENTIRE repository must be cloned to work.
* While there are redundancies for if the bash commands are run from the `MAIN` repository, you should generally run them in the 'as7' folder.
* All testing of commands were done in WSL on Windows 11 on different devices and with fresh repository clones.

Additionally, all instructions are made with the following assumptions:

* The user starts in the assignment (`../as7` or otherwise) directory.
* The user already has most dependencies installed.
* The user has (by default) cloned the repository normally.

## INSTRUCTIONs [FEATURE \#11 (4 pts)]

### [OPTIONAL] DEPENDENCIES

To run this project on most systems, including WSL, ensure you have the following dependencies listed in the following command (or just run it :p)...

```bash
sudo apt get-update # updates sudo
sudo apt install cmake g++ libgl-dev libwayland-dev wayland-protocols libxrandr-dev pkg-config libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev mesa-utils build-essential cmake xorg-dev pulseaudio
sudo apt --fix-broken install # should fill any missing dependencies
```

### [OPTIONAL] RECURSIVELY CLONING THE REPOSITORY

You can normally clone the repository, but doing so will incur some extra work to be done for fetching submodules. If you don't want to do that, use the following commands in your target directory...

```bash
git clone --recurse-submodules https://github.com/PieFlavr/CS381
```

### FETCHING SUBMODULEs

If you cloned the repository normally without `--recurse-submodules` or there are issues with the submodules, they must be fetched manually.
To fetch them manually, from the assignment directory (`../as7`), run ALL the following...

```bash
git submodule init 
git submodule update --init --recursive 
```

### COMPILING THE CODE

In order, either from the main (`../build`) or assignment (`../as7`) repository directory, simply run ALL following commands in the terminal...

```bash
cd as7 ; cd build # Works from main/assignmnent/build folds. IGNORE ERRORS FROM THIS!!!
cmake ..   # Generates makefile + fixes dependencies
make   # Generates as7 executable, must run every time code is changed.
```

`[OPTIONAL]` If running into a wayland-related error during `cmake`, go into `CMakeCache.txt` and set this setting to OFF `GLFW_BUILD_WAYLAND:BOOL=OFF`.
This may or may not disable *window decorations*, but by default without the above modification they should be visible and enabled.

### RUNNING THE CODE

To run the code after compilation, simply run the `as7` executable from the `../build/` directory via running ALL the following commands...

```bash
./as7   # Runs program executable
```

### [OPTIONAL] BUILD/RUNNING ERRORs

If unable to compile, run, or getting errors with regards to directories/pathing, it is recommended you delete the contents of the `build` folder and re-compile the code. Make sure you are in the BUILD directory of the assignment (command accounts for that but still be careful)!!!

```bash
cd as7 ; cd build # Should work from main, assignment, or build directory... IGNORE ERRORS FROM THIS (accounts for being in either main/assignment/build directory)
rm -rf ./*   # Deletes the contents of the build folder
```

Additionally, if receivng a `DRIVER TIMOUT` or some sort of `DRIVER CRASH`, it is recommended to simply restart your computer to fix it along with the above.

## APPLICATION CONTROLs

All Kinematics is "smoothed" via acceleration and turning speed parameters unique to each entity. All entities "accelerate" to their target speeds and/or headings appropriately.

`[UP_ARROW]` and `[DOWN_ARROW]` increments the entity's target heading up/down respectively.

`[A]` and `[D]` increments the entity's target heading left/right respectively.

`[W]` and `[S]` increments the car's target speed up/down respectively.

`[SPACE]` sets the car's target speed to zero.

## FEATURE IMPLEMENTATIONs

The following base features have been implemented in the program...

1) [X] Each Entity should be stored in a list-like structure and their behavior should be implemented as separate components stored in a list in each entity (max score 60% if violated).
2) [X] Create a window titled “CS381 - Assignment 6” using Raylib.
3) [X] Create a scene with five different types of cars and a rocket sitting on the ground behind each (all facing towards the left or right of the camera), a ground plane, and a skybox.
4) [X] Every car should have different physics properties (maximum speed, acceleration, turning rate) (10 points each, 50 points total).
5) [X] Similarly the rockets should have different physics properties from the cars, but all the rockets can use the same properties (5 points).
6) [X] Implement a transform component to store transforms and a rendering component to draw meshes on the screen (should be able to toggle a bounding box on or off) (5 points).
7) [X] Implement a physics component (or two, inheriting behavior from components could be useful for extra credit purposes) which implements basic kinematic physics (position + = velocity ∗ dt) (5 points) and 2D oriented physics based on a heading (5 points).
8) [X] Create a selection management system, where only the selected entity has a bounding box drawn around it and pressing tab selects the next entity (5 points).
9) [X] Implement an input component which uses BufferedRaylib (buffered input, 10 points) to control the selected entity (5 points total), please use incrementing, not continuous, values and each entity should continue to move even when not actively controlled or selected (max score 60% if violated).
[X] Pressing W smoothly increases the selected entity’s velocity (easier to implement as a speed) in the direction it's facing (1 point).
[X] Pressing S smoothly decreases the selected entity’s velocity in the direction it's facing (1 point).
[X] Pressing A smoothly increases the selected entity’s heading (1 point)
[X] Pressing D smoothly decreases the selected entity’s heading (1 point)
[X] Pressing Space should smoothly reset the selected entity’s velocity to zero (1 point).

10) [X] Your as7 folder in your Github repository includes a readme file that should:
[X] Describe how to fetch your git submodules with commands that can be copied and pasted into a Linux terminal.
[X] Describe how to compile your code with copy and pasteable commands.
[X] Describe how to run your code and enumerate its controls.
[X] If you have any extra credit be sure to mention it so we don't miss it!

11) [X] Readme "Question": Please implement your assignment using CO.hpp Download CO.hpp(feel free to make some minor adjustments if you would like). This file should be thoroughly commented, I'm expecting at least one line of comment per line of code (10 points).

## EXTRA CREDIT FEATURE IMPLEMENTATIONs

The following have been implemented in the program...

1) [ ] Add an engine noise to the car, or wind howling in the background (+5 points).
2) [X] Make the game third person, with a camera that follows the selected entity (+10 points).
3) [X] Don't use CO.hpp instead implement your own version (this file should still be thoroughly commented) (+5 points)
4) [ ] Stop rockets from being able to pass through the ground or other entities (hint: look at the methods on raylib bounding boxes) (+10 points)
5) [X] Implement a scene hierarchy and add wheels that are children of their cars (+10 points)
