Lua API
=======

Overview
--------

Most of the Lua interfaces provided are documented in C++ source files.

- Classes
	- Audio
	- Font
	- Texture
	- MIDIIn
	- MIDIOut
- Modules
	- Graphics
	- Geometry
	- Util
	- UTF-8
	- Window
	- Input

Compatibility with vanilla Lua
------------------------------

- bit
	- This is an extension from LuaJIT.
- utf8
	- Almost compatible with Lua 5.3, but error conditions are reduced.
- os
	- os.exit is replaced with a C function that also cleans up resources.
