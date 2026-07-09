# SDL3Game

A simple "game" made with SDL3 (and SDL2).

The code is a thin wrapper around SDL with separate implementations for SDL3 and SDL2.

Additional wrappers are available in the `/libs directory/`.  SDL3 provides more functionality than SDL2, so any descrepancies are implemented with C++ library functions.

### Features

* Window
* Keyboard
* Mouse
* Game controller
* Joystick
* Audio

### Platforms

* Sample executables are available in `/release_exe/` for multiple platforms
* SDL3 executables built with Zig lack audio funtionality as Zig does not yet support SDL3_Mixer
* Applications can be build by using/modifying the makefiles in `/game_io_test/src/pltfm/`
