# CS381-AS0

The following is (temporarily) for personal use maintaining the project proper!

## How to Add and Update Submodules

For this project, most relevant to the <https://github.com/joshuadahlunr/raylib-cpp> submodule. 

```bash
git submodule add <"submodule-link"> .../<"target-directory">/<"submodule-folder-name">
git submodule init # From Main Directory
git submodule update --init --recursive
```

## WSL Problems & Solutions

### Required Dependencies

```bash
sudo apt get-update # updates sudo
sudo apt install cmake g++ libgl-dev libwayland-dev wayland-protocols libxrandr-dev pkg-config libxkbcommon-dev libxinerama-dev libxcursor-dev libxi-dev mesa-utils build-essential cmake xorg-dev pulseaudio
sudo apt --fix-broken install # should fill any missing dependencies
```

### Audio Issues

While it should already be covered by the dependencies, refer to this if audio issues occur.

Raylib audio (theoretically) works perfectly fine inside of WSL itself, but the issue comes with piping the audio from WSL to the outer Windows computer. This is easily solved via the use of PulseAudio. Using the following solves any theoretical issues running this otherwise...

```bash
sudo apt get-update #usually not updated enough to handle
sudo apt install pulseaudio
```

If any further issues are encountered with playing/manipulation audio, refer to `pulseaudio` for attempted solutions.
