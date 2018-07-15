Lua API
=======

Overview
--------

Most of the Lua interfaces provided are documented in C++ source files.

- Classes
	- Audio
	- Font
	- Texture
	- IOStream
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

- OOP
	- Create an object by `Class.new(…)`.
	- Call a method by `instance.method(…)` rather than `instance:method(…)`.
	- These new conventions of course don't work on built-in Lua objects like string and file; however, file is replaced by File (see below), which follows the new convention.
- bit
	- This is an extension from LuaJIT.
- utf8
	- Almost compatible with Lua 5.3, but error conditions are reduced.
- file
	- This class isn't useful because io.open, io.popen, io.tmpfile and io.close are removed. io.stdin, io.stdout and io.stderr are still there.
	- Use IOStream, which is capable of handling UTF-8 filenames, instead if you would like to read or write files.
- os
	- os.exit is replaced with a C function that also cleans up resources.
