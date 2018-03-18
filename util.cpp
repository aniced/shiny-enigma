namespace Util {
	//-------------------------------------------------------------------------
	// ● to_rect
	//   Unspecified values will be set to zero.
	//-------------------------------------------------------------------------
	void to_rect(lua_State* L, int index, SDL_Rect* rect) {
		lua_getfield(L, index, "x"); rect->x = lua_tointeger(L, -1);
		lua_getfield(L, index, "y"); rect->y = lua_tointeger(L, -1);
		lua_getfield(L, index, "w"); rect->w = lua_tointeger(L, -1);
		lua_getfield(L, index, "h"); rect->h = lua_tointeger(L, -1);
		lua_pop(L, 4);
	}
	//-------------------------------------------------------------------------
	// ● create_rect
	//-------------------------------------------------------------------------
	void create_rect(lua_State* L, const SDL_Rect* rect) {
		lua_createtable(L, 0, 4);
		lua_pushnumber(L, rect->x); lua_setfield(L, -2, "x");
		lua_pushnumber(L, rect->y); lua_setfield(L, -2, "y");
		lua_pushnumber(L, rect->w); lua_setfield(L, -2, "w");
		lua_pushnumber(L, rect->h); lua_setfield(L, -2, "h");
	}
	//-------------------------------------------------------------------------
	// ● to_point
	//-------------------------------------------------------------------------
	void to_point(lua_State* L, int index, SDL_Point* point) {
		lua_getfield(L, index, "x"); point->x = lua_tointeger(L, -1);
		lua_getfield(L, index, "y"); point->y = lua_tointeger(L, -1);
		lua_pop(L, 2);
	}
	//-------------------------------------------------------------------------
	// ● to_color
	//   Unspecified alpha will be set to 255.
	//-------------------------------------------------------------------------
	void to_color(lua_State* L, int index, SDL_Color* color) {
		lua_getfield(L, index, "r"); color->r = lua_tointeger(L, -1);
		lua_getfield(L, index, "g"); color->g = lua_tointeger(L, -1);
		lua_getfield(L, index, "b"); color->b = lua_tointeger(L, -1);
		lua_getfield(L, index, "a"); color->a = luaL_optinteger(L, -1, 255);
		lua_pop(L, 4);
	}
	//-------------------------------------------------------------------------
	// ● create_color
	//-------------------------------------------------------------------------
	void create_color(lua_State* L, const SDL_Color* color) {
		lua_createtable(L, 0, 4);
		lua_pushnumber(L, color->r); lua_setfield(L, -2, "r");
		lua_pushnumber(L, color->g); lua_setfield(L, -2, "g");
		lua_pushnumber(L, color->b); lua_setfield(L, -2, "b");
		lua_pushnumber(L, color->a); lua_setfield(L, -2, "a");
	}
}
