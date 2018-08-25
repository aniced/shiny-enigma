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

#ifdef __WINDOWS__
	#undef WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#define ENABLE_MIDI
#endif
#include "SDL_syswm.h"

#include "global.cpp"
#include "util.cpp"
#include "object.cpp"
#include "utf8.cpp"
#include "iostream.cpp"
#include "geometry.cpp"
#include "font.cpp"
#include "graphics.cpp"
#include "texture.cpp"
#include "window.cpp"
#include "input.cpp"
#include "audio.cpp"
#ifdef ENABLE_MIDI
	#ifdef __WINDOWS__
		#include "midiin_windows.cpp"
		#include "midiout_windows.cpp"
	#endif
#endif
#ifdef __WINDOWS__
	#include "dialog_windows.cpp"
#else
	#include "dialog_sdl.cpp"
#endif
#include "base.cpp"
