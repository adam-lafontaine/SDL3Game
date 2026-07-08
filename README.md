# SDL3Game

A simple "game" made with SDL3 (and SDL2)

The code is a thin wrapper around SDL with separate implementations for SDL3 and SDL2.  The application demonstrates how to use...
* Window
* Keyboard
* Mouse
* Game controller
* Joystick
* Audio

Additional wrappers are available in the `/libs directory/`.  SDL3 provides more functionality than SDL2, so any descrepancies are implemented with C++ library functions.

Sample executables are available in `/release_exe/`.  There are multiple targets built with the Zig build system.  Unfortunately audio is disabled in the executables because the SDL3_Mixer library is not yet available for static linking with Zig.  However, fully functional applications can be built using the makefile in `/game_io_test/src/pltfm/`.
