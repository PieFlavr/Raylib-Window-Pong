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

## INSTRUCTIONs

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

All of the controls are enumerated in the menus/scoreboards. The following is simply an explicit listing of them, all of these can be found in the game.

### IMPLEMENTED

IN THE GAME ONLY...

`[W]` and `[S]` moves the left paddle up and down respectively.

`[UP]` and `[DOWN]` arrow keys move the right paddle up and down respectively.

`[ESC]` at any point closes the game.

### NOT-IMPLEMENTED

IN THE START MENU ONLY...

`[T]` in the starting menu shows the leaderboard.

IN THE MENU AND IN-GAME...

`[SPACE]` holds the scoreboard window and enlarges it.

`[MIDDLE_CLICK]` on the scoreboard window while enlarged let's you move it.

IN THE START MENU AND GAME-OVER SCREEN...

`[ENTER]` re/starts the main game.

## FEATURE IMPLEMENTATIONs

The following base features have been implemented in the program...

1) [X] Implemented using OOEC (0pts)
2) [X] Multiple Entities (10pts)
3) [X] Controls (20pts)
4) [X] Raylib (1pts) (technically a fork but still raylib :p)
5) [X] C++ (1pts)
6) [X] Have a Window (10pts)
7) [X] Two Different Components Interact (15pts)
8) [X] Two Different Entities Interact (20pts)
9) [X] Readme (5pts)
10) [X] Load and Use Something (8pts)
11) [X] Use At Least One Event (10pts)

## EXTRA CREDIT FEATURE IMPLEMENTATIONs

The following have been implemented in the program...

1) [X] Free Points (+7pts)
2) [X] Don't Use CO.hpp (+50pts)
3) [X] Efficient Component Type (+1pts per; total +24pts)
4) [X] Scene Tree (+10pts)
5) [] Spatial Audio (+10pts directional; +5pts spatial)
6) [X] Extra Windows (+1pt per extra; total +4pts, not counting those in unused folder!)
7) [] Graph Algorithms (+10pts)
8) [X] Calculate Your Own DT (+2pts)
9) [] Do Your Own Voice Acting (+10pts)
10) [X] Add Firelink Shrine (+1pts)
11) [X] Guess Your Grade (+5pts) (198pts (w/o bonus) + 5pts = 203pts (w/ bonus guess))
