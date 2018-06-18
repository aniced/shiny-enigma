//=============================================================================
// ■ everything.cpp
//-----------------------------------------------------------------------------
//   The only C++ source file to be compiled.
//=============================================================================

#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <cstdlib>
#include <cstdio>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "global.cpp"
#include "util.cpp"
#include "utf8.cpp"
#include "geometry.cpp"
#include "graphics.cpp"
#include "texture.cpp"
#include "window.cpp"
#include "input.cpp"
#include "audio.cpp"
#include "base.cpp"
