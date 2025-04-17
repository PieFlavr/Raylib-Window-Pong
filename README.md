# RAYLIB WINDOW PONG README

This is a repository containing a personal fork of an assignment from a CS381 class. This uses an experimental fork of raylib which allows for management of multiple windows. As a challenge and for extra credit in the class, at base this is coded without ANY objects/structs/components or similar, with every coded "Ad Hoc" withou any real architecture or structure. As an added challenge, I did my best to try and avoid functions (with some exception) and use primarily macros for what would have been delegated to "object-oriented" behavior.

## PREFACE

This project is likely structured slightly differently than similar projects. The following are key important details:

* There is a deliberate pre-existing empty (with exception of .gitkeep) `build` folder in each assignment folder.
* All assets folders and submodules such as `raylib-cpp` exist in the main directory (there are no copies per assignment directory).
* The ENTIRE repository must be cloned to work.
* While there are redundancies for if the bash commands are run from the `MAIN` repository, you should generally run them in the 'as4' folder.
* All testing of commands were done in WSL on Windows 11 on different devices and with fresh repository clones.

Additionally, all instructions are made with the following assumptions:

* The user starts in the assignment (`../as4` or otherwise) directory.
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
git clone --recurse-submodules https://github.com/PieFlavr/Raylib-Window-Pong
```

### FETCHING SUBMODULEs

If you cloned the repository normally without `--recurse-submodules` or there are issues with the submodules, they must be fetched manually.
To fetch them manually, from the assignment directory (`../as4`), run ALL the following...

```bash
git submodule init 
git submodule update --init --recursive 
```

### COMPILING THE CODE

In order, either from the main (`../build`) or assignment (`../as4`) repository directory, simply run ALL following commands in the terminal...

```bash
cd as4 ; cd build # Works from main/assignmnent/build folds. IGNORE ERRORS FROM THIS!!!
cmake ..   # Generates makefile + fixes dependencies
make   # Generates as4 executable, must run every time code is changed.
```

`[OPTIONAL]` If running into a wayland-related error during `cmake`, go into `CMakeCache.txt` and set this setting to OFF `GLFW_BUILD_WAYLAND:BOOL=OFF`.
This may or may not disable *window decorations*, but by default without the above modification they should be visible and enabled.

### RUNNING THE CODE

To run the code after compilation, simply run the `as4` executable from the `../build/` directory via running ALL the following commands...

```bash
./as4   # Runs program executable
```

Currently, incompatible with windows due to compilation issues with the multi-window raylib release.

### [OPTIONAL] BUILD/RUNNING ERRORs

If unable to compile, run, or getting errors with regards to directories/pathing, it is recommended you delete the contents of the `build` folder and re-compile the code. Make sure you are in the BUILD directory of the assignment (command accounts for that but still be careful)!!!

```bash
cd as4 ; cd build # Should work from main, assignment, or build directory... IGNORE ERRORS FROM THIS (accounts for being in either main/assignment/build directory)
rm -rf ./*   # Deletes the contents of the build folder
```

Additionally, if receivng a `DRIVER TIMOUT` or some sort of `DRIVER CRASH`, it is recommended to simply restart your computer to fix it along with the above.

## APPLICATION INFORMATION

The game itself is relatively simple, it is just cooperative 'PONG' with cool multi-windows magic and yelling for power. The main window in the middle keeps moving around, while two smaller windows follow the paddles on the left/right. The main window bounces around randomly and the added challenge is the fact most of the screen is obscured. That, and goofy window management as well.

On startup, it might black out for a bit but just give it a minute and it'll be fine. You can see the start menu, which has instructions and all, and once you start the actual game all the gravity flips for the scoreboard and the rest go into place. After that, it's just a matter of survival, you get minus timer for letting the ball hit the sides the paddles are protecting, but you gain timer if you bounce it correctly. And yeah, smaller aspect ratios can absolutely cheese this since it uses the top-left monitor to do the logic.

Surprisingly, works on multi-monitor steups! Can't do fancier mouse stuff though since I'd need to directly use system calls to get mouse positions and such (which is system dependent since raylib won't let you do that) so kind of stuck with this bootleg control scheme.

After the game ends, if you get a high enough score, you can enter your name to admit it to the leaderboard! And after that, you can play again. At any time, you can just press `[ESC]` to exit the game entirely.

### APPLICATION CONTROLs

All of the controls are enumerated in the menus/scoreboards. The following is simply an explicit listing of them, all of these can be found in the game. 

IN THE START MENU ONLY...

`[T]` in the starting menu shows the leaderboard.

IN THE MENU AND IN-GAME...

`[SPACE]` holds the scoreboard window and enlarges it.

`[MIDDLE_CLICK]` on the scoreboard window while enlarged let's you move it.

IN THE GAME ONLY...

`[W]` and `[S]` moves the left paddle up and down respectively.

`[UP]` and `[DOWN]` arrow keys move the right paddle up and down respectively.

`[SHOUTING]` or `[BEING LOUD]` given that you have a microphone speeds the game up!

IN THE START MENU AND GAME-OVER SCREEN...

`[ENTER]` re/starts the main game.

`[ESC]` at any point closes the game.

IN THE GAME OVER SCREEN...

`[SHIFT]` to submit your typed name

### ASSET USE

The following music tracks were used to create this project...

Lucky Star: https://youtu.be/1hbHgZDB95o?si=YvHRP71iqUQcIBFy 
Cats on Mars: https://youtu.be/97xfV6yXcrk?si=uDQGLkleoicX_xSb 
Determination: https://youtu.be/W1i4mTyidOc?si=mzxLFXQke-5vfmj2 
