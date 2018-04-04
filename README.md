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

### Windows

- MinGW-w64 GCC is the only tested compiler.
- You need to link with `-lSDL2main` in addition to the linker command above.

### \*nix

- This is not tested.
- In theory, this project can run smoothly on these platforms. sdl2-config should be called to get the right command line.
