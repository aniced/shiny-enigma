namespace Window {
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
	// ● set_title(title)
	//-------------------------------------------------------------------------
	int set_title(lua_State* L) {
		const char* text = luaL_checkstring(L, 1);
		SDL_SetWindowTitle($window, text);
		return 0;
	}
	void init() {
		const luaL_reg reg[] = {
			{"resize", resize},
			{"set_title", set_title},
			{NULL, NULL}
		};
		luaL_register(L, "Window", reg);
		lua_pop(L, 1);
	}
}
