# (untitled) #

Compiling
---------

### Prerequisites

- Make sure the project path involves only printable ASCII characters. To be safe, space is not allowed.
	- On platforms with good native UTF-8 support, it may run in directories with special characters, but it's not tested.
	- As things become complex and tricky when SDL's UTF-8 and multibyte character encodings coexist, keeping it simple is much more important.
- Install the libraries so that you can compile with some command like `gcc some.c -llua51 -lSDL2_image -lSDL2_ttf -lSDL2`. The tested versions are listed below.
	- LuaJIT 2.0.5
	- SDL2 2.0.8
	- SDL2\_image 2.0.3
	- SDL2\_ttf 2.0.14
- Prepare a TrueType font in the project root directory called ‘fon0.ttf’.

### For all platforms

- Compile everything.cpp with the required libraries linked as usual.
- The following sections contains platform-specific notes.

### Windows

- MinGW-w64 GCC is the only tested compiler.
- You need to link with `-lSDL2main` in addition to the linker command above.

### \*nix

- GCC 7.3 is not going to work
- You need clang with -std=gnu++11
- clang everything.cpp `sdl2-config --cflags --libs` `pkg-config --cflags --libs luajit SDL2_image SDL2_ttf` -o shiny -std=gnu++11
- And something in lua will get wrong probably
