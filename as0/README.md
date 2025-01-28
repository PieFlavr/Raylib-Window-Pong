**(1A) Include a 2-3 sentence explanation of how to make a window resizable (5 points)...**
asd

**(1B) ...And a 2-3 sentence explanation of how to keep text centered in a resizing window (5 points).**
asd

**(2) Describe how to fetch your git submodules (5 points) with commands that can be copy and pasted into a Linux terminal (5 points).**
asd
```
```

**(3) Describe how to compile your code (10 points) with copy and pasteable commands (10 points).**
Ensure you have the following dependencies listed as follows, or simply run the following command.
```
sudo apt install cmake g++ libgl-dev libwayland-dev wayland-protocols libxrandr-dev pkg-config libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev mesa-utils
```
Then, in order from the main repository directory, simply run the following commands in the terminal in order.
```
cd as0/build
cmake ..
make
```
If running into a wayland-related error during `cmake`, go into `CMakeCache.txt` and set this setting to OFF `GLFW_BUILD_WAYLAND:BOOL=OFF`.

**(4) Describe how to run you’re code (10 points) and enumerate its controls [I predict "not applicable" will be the most common set of controls for this assignment.] (10 points).**
asd
```
```