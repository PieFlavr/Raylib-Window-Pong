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

1) liber nostrum

## EXTRA CREDIT FEATURE IMPLEMENTATIONs

The following have been implemented in the program...

1) liber nostrum

## KNOWN ISSUES (to-do-list)

1) Skybox needs to be re-implemented without raylib-cpp.
