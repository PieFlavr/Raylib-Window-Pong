# AS1 README

## PREFACE

This project is likely structured slightly differently then similar projects. The following are key important details:
* There is a deliberate pre-existing empty (with exception of .gitkeep) `build` folder in each assignment folder.
* All assets folders and submodules such as `raylib-cpp` exist in the main directory (there are no copies per assignment directory).
* The ENTIRE repository must be cloned to work. 
* While there are redundancies for if the bash commands are run from the `MAIN` repository, you should generally run them in the 'AS1' folder.

Additionally, all instructions are made with the following assumptions:
* The user starts in the assignment (`../as1` or otherwise) directory.
* The user already has most dependencies installed.
* The user has (by default) cloned the repository normally. 

## INSTRUCTIONs [FEATURE \#7 (9 pts)]

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

Now, your folder structure shoould look similar to this...

![alt text](../assets/images/folder-example-as1.png)

### FETCHING SUBMODULEs

If you cloned the repository normally without `--recurse-submodules` or there are issues with the submodules, they must be fetched manually.
To fetch them manually, from the assignment directory (`../as1`), run ALL the following...

```bash
git submodule init 
git submodule update --init --recursive 
```

### COMPILING THE CODE

In order, either from the main (`../build`) or assignment (`../as1`) repository directory, simply run ALL following commands in the terminal...

```bash
cd as1 ; cd build # Works from main/assignmnent/build folds. IGNORE ERRORS FROM THIS!!!
cmake ..   # Generates makefile + fixes dependencies
make   # Generates as1 executable, must run every time code is changed.
```

`[OPTIONAL]` If running into a wayland-related error during `cmake`, go into `CMakeCache.txt` and set this setting to OFF `GLFW_BUILD_WAYLAND:BOOL=OFF`.
This may or may not disable *window decorations*, but by default without the above modification they should be visible and enabled.

### RUNNING THE CODE

To run the code after compilation, simply run the `as1` executable from the `../build/` directory via running ALL the following commands...

```bash
./as1   # Runs program executable
```

### [OPTIONAL] BUILD/RUNNING ERRORs

If unable to compile, run, or getting errors with regards to directories/pathing, it is recommended you delete the contents of the `build` folder and re-compile the code. Make sure you are in the BUILD directory of the assignment (command accounts for that but still be careful)!!!

```bash
cd as1 ; cd build # Should work from main, assignment, or build directory... IGNORE ERRORS FROM THIS (accounts for being in either main/assignment/build directory)
rm -rf ./*   # Deletes the contents of the build folder
```

## APPLICATION CONTROLs

Controlling the application is very simple.
Pressing `[TAB]` will highlight a volume slider, and pressing it again will cycle through each slider. The highlight should look as follows...
(left = light mode ; right = dark mode)

![alt text](../assets/images/cs1-light-highlight-example.png)   ![alt text](../assets/images/cs1-dark-highlight-example.png)

To change the highlighted volume slider, simply press the `[LEFT]` arrow key to decrease volume or `[RIGHT]` to increase it. 

You can simply `[LEFT_CLICK]` on the `PING` button with your mouse to play a sound effect (affected by SFX Volume).
To change the theme appearance, you just `[LEFT_CLICK]` on the `THEME TOGGLE` button,

## QUIZ QUESTIONs and ANSWERs [FEATURE \#8 (10 pts)]

The following are the answer to the quiz questions required for the assignment.

**(1) Explain at a high level how audio is produced by a speaker. Why is the raylib::AudioDevice necessary in your code? (10 pts)**

At a high level, the speaker produces audio by converting electrical signals to vibrations/oscillations in the air that we interpret as sound. The waveform of the electrical signal controls the speaker device generating these vibrations, whether that speaker is essentially just a diaphragm, solenoid, or otherwise. 

When playing a piece of audio such as an .mp3 or .wav, samples or even chunks of samples are sent to the speaker. For smaller more immediate sound bites (SFX), these can be loaded into RAM for speed, but longer files (Music) they can be streamed from the disk to the RAM instead in chunks. Either way, they are sent to the speaker. 

From looking at the terminal messages, the `raylib::AudioDevice` is necessary in the code because this essentially sets up the audio engine and resource management needed to actually play the audio. Without it, we would essentially be attempting to play audio without an interface to actually manipulate the speaker. 

## EXTRA CREDIT FEATURE IMPLEMENTATIONs

The following have been implemented in the program...

1) Dark Background Option (5 points)
2) Custom Audio Off the Internet (5 points)
3) Arrow key Controls (5 points)
4) Slider Tab Switch (5 points)
5) Dark Theme GUI (5 points)

In the source code `as1.cpp` you can [CTRL+F] and type "EC Feature" to see all primary implementations of the above.