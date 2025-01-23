# Setting up the CS381 Enviornment0

0) Make sure you have access to a linux machine or the class virtual machine: https://github.com/sushillouis/381/wiki/Setting-up-a-CS381-VM

1) Install GCC (should be there by default on any linux enviornment) and Visual Studio Code (https://code.visualstudio.com/download)

**Untested:** If using WSL also make sure you are using Ubuntu 22.04 and you have a the linux GUI development tools installed (`sudo apt install build-essential cmake git xorg-dev`), this should bring all of the needed windowing dependencies with it: https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps

2) Create a folder to house your repository.

3) Run `git init` in this folder.

4) Create a private GitHub repository (https://www.youtube.com/watch?v=znRVAvQy5FU) and add `joshuadahlunr` and `Znasif` as contributers.

5) Use `git remote add origin <your project's .git address>` in that folder to make it a repository.

6) Create a folder for assignment 0 (`mkdir as0 && cd as0`)

7) Add Raylib as a submodule in that folder (`git submodule add https://github.com/joshuadahlunr/raylib-cpp.git`)

8) Clone all of Raylib's dependencies (`git submodule init` and `git submodule update --init --recursive`)

9) Open Visual Studio Code in the as0 folder (`code .`)

10) Install the C/C++ Extension Pack (four boxes with one offset on the left navigation bar, search for C++, it should be near the top)

11) Create a new folder called `src` and a new `helloworld.cpp` file in that folder.

12) Write hello world in helloworld.cpp

13) Download `CMakeLists.txt` off of canvas and move it into your as0 folder: https://webcampus.unr.edu/files/13113431/download?download_frd=1

14) You may need to replace the line `add_subdirectory(raylib)` with `add_subdirectory(raylib-cpp)`

15) Create a new folder in as0 called `build` (`mkdir build`)

16) Inside build (`cd build`) run `cmake .. && make` to compile your libraries

17) If everything built with no errors you should be able to run `./as0` and see the results of your hello world application printed to the screen.