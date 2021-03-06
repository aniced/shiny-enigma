//=============================================================================
// ■ Window
//-----------------------------------------------------------------------------
//   A Lua module for managing the application window.
//=============================================================================

namespace Window {
	//-------------------------------------------------------------------------
	// ● resize(width, height)
	//-------------------------------------------------------------------------
	int lua_resize(lua_State* L) {
		int w = luaL_checkinteger(L, 1);
		int h = luaL_checkinteger(L, 2);
		SDL_SetWindowSize($window, w, h);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_resizable(resizable)
	//-------------------------------------------------------------------------
	int lua_set_resizable(lua_State* L) {
		luaL_checkany(L, 1);
		SDL_SetWindowResizable($window, (SDL_bool) lua_toboolean(L, 1));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_fullscreen(mode)
	//   mode: "fullscreen", "borderless" or "windowed"
	//-------------------------------------------------------------------------
	int lua_set_fullscreen(lua_State* L) {
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
			Util::sdlerror(L, "SDL_SetWindowFullscreen() < 0");
		}
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_title(title)
	//-------------------------------------------------------------------------
	int lua_set_title(lua_State* L) {
		const char* text = luaL_checkstring(L, 1);
		SDL_SetWindowTitle($window, text);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"resize", lua_resize},
			{"set_resizable", lua_set_resizable},
			{"set_title", lua_set_title},
			{"set_fullscreen", lua_set_fullscreen},
			{NULL, NULL}
		};
		luaL_register(L, "Window", reg);
		lua_pop(L, 1);
	}
}
