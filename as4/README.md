# "MY HATRED FOR THIS CODE IS IMMEASURABLE, AND MY DAY IS RUINED (the game)"[as4] README

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

### COMPLAINING

The raymarching shaders may or may not work, its hard testing on WSL on a such a short time-frame but it is what it is. When it does, it should look pretty cool! Same with the music, still haven't figured out why I'm getting buffering issues on my devices but from what I could hear it's probably cool.

Audio input was surprisingly fine, and wasn't too bad to work with to measure loudness. The Terminal ASCII art (audio visualizer) was also pretty easy! Even though at some point it started spitting out memory corruption demons because I was mis-indexing the 1D encoding (no arrays/lists/structs/objects challenge), it was fine.

The worst part of this is most definetely getting the god-forsaken Raylib fork to work, and subsequently managing all the windows to get it as close as possible to what I want (i.e. multiple viewports). Ended up having to do some napkin math since for some reason Raylib doesn't let you mess with the projection matrix directly so I couldn't do the easy math and had to opt for ridiculous camera teleportation and such.

The other worst part of that whole ordeal was managing multiple windows, because they weren't object-like and instead just acted as a context switch, and not even an entirely great one at that because some random properties persist along the way that mess everything up if you don't specifically account for it. The most cursed, and I say the MOST cursed part of this whole project was discovering the fact I could use MACROS in combination with SCOPE OPERATORs (do-while) to bootleg "object-oriented" behaviors. I have never seen anything this cursed in my time on this earth, and I could not believe I was capable of such a crime against humanity.

tl;dr I don't think I should be allowed to code again after this.

### APPLICATION CONTROLs

IN THE START MENU ONLY...

`[T]` in the starting menu shows the leaderboard.

IN THE MENU AND IN-GAME...

`[SPACE]` holds the scoreboard window and enlarges it.

`[MIDDLE_CLICK]` on the scoreboard window while enlarged let's you move it.

IN THE GAME ONLY...

`[W]` and `[S]` moves the left paddle up and down respectively.

`[UP]` and `[DOWN]` arrow keys move the right paddle up and down respectively.

IN THE START MENU AND GAME-OVER SCREEN...

`[ENTER]` re/starts the main game.

`[ESC]` at any point closes the game.

## EXTRA CREDIT FEATURE IMPLEMENTATIONs

The following have been implemented in the program...

1) Multiplayer (Left/Right Player Paddles)
2) Audio Input (Yelling Accelerator)
3) Terminal ASCII Art (Audio Input Visualizer)
4) Funny Name (see README title)
5) Manual Cleanup (literally using a random Raylib fork, not the CPP wrapper ￣へ￣)
6) Custom Shaders (sometimes works, sometimes doesnt')
7) Leaderboard Implementation (persists + lets you type name)
