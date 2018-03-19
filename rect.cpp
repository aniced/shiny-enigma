//=============================================================================
// ■ Rect
//-----------------------------------------------------------------------------
//   A module for dealing with rectangles in Lua.
//=============================================================================

namespace Rect {
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
}
