# (untitled) #

Compiling
---------

### Prerequisites

- Install the libraries so that you can compile with some command like `gcc some.c -llua51 -lSDL2_image -lSDL2_ttf -lSDL2`. The tested versions are listed below.
	- LuaJIT 2.0.5
	- SDL2 2.0.8
	- SDL2\_image 2.0.3
	- SDL2\_ttf 2.0.14
- Prepare a TrueType font in the project root directory called ‘fon0.ttf’.

### Windows

- MinGW-w64 GCC is the only tested compiler.
- You need to link with `-lSDL2main` in addition to the linker command above.
- Compile everything.cpp as usual.

### \*nix

- This is not tested.
- In theory, this project can run smoothly on these platforms. sdl2-config should be called to get the right command line.
- Compile everything.cpp as usual.
