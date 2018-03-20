//=============================================================================
// ■ Window
//-----------------------------------------------------------------------------
//   A Lua module for managing the application window.
//=============================================================================

namespace Window {
	//-------------------------------------------------------------------------
	// ● get_rect()
	//   Use this size for drawing.
	//-------------------------------------------------------------------------
	int get_rect(lua_State* L) {
		SDL_Rect rect = {0, 0, -1, -1};
		SDL_GetRendererOutputSize($renderer, &rect.w, &rect.h);
		Rect::create_rect(L, &rect);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● resize(width, height)
	//-------------------------------------------------------------------------
	int resize(lua_State* L) {
		int w = luaL_checkinteger(L, 1);
		int h = luaL_checkinteger(L, 2);
		SDL_SetWindowSize($window, w, h);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_fullscreen(mode)
	//   mode: "fullscreen", "borderless" or "windowed"
	//-------------------------------------------------------------------------
	int set_fullscreen(lua_State* L) {
		const char* mode = luaL_checkstring(L, 1);
		Uint32 flags;
		if (strcmp(mode, "fullscreen") == 0) {
			flags = SDL_WINDOW_FULLSCREEN;
		} else if (strcmp(mode, "borderless") == 0) {
			flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		} else if (strcmp(mode, "windowed") == 0) {
			flags = 0;
		} else {
			return luaL_error(L, "unknown fullscreen mode");
		}
		if (SDL_SetWindowFullscreen($window, flags) < 0) {
			error("SDL_SetWindowFullscreen() < 0");
		}
	}
	//-------------------------------------------------------------------------
	// ● set_title(title)
	//-------------------------------------------------------------------------
	int set_title(lua_State* L) {
		const char* text = luaL_checkstring(L, 1);
		SDL_SetWindowTitle($window, text);
		return 0;
	}
	void init() {
		const luaL_reg reg[] = {
			{"get_rect", get_rect},
			{"resize", resize},
			{"set_title", set_title},
			{"set_fullscreen", set_fullscreen},
			{NULL, NULL}
		};
		luaL_register(L, "Window", reg);
		lua_pop(L, 1);
	}
}
