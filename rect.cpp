//=============================================================================
// ■ Rect
//-----------------------------------------------------------------------------
//   A Lua module for dealing with rectangles.
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
	// ● check_rect
	//-------------------------------------------------------------------------
	void check_rect(lua_State* L, int index, SDL_Rect* rect) {
		lua_getfield(L, index, "x"); rect->x = luaL_checkinteger(L, -1);
		lua_getfield(L, index, "y"); rect->y = luaL_checkinteger(L, -1);
		lua_getfield(L, index, "w"); rect->w = luaL_checkinteger(L, -1);
		lua_getfield(L, index, "h"); rect->h = luaL_checkinteger(L, -1);
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
	// ● check_point
	//-------------------------------------------------------------------------
	void check_point(lua_State* L, int index, SDL_Point* point) {
		lua_getfield(L, index, "x"); point->x = luaL_checkinteger(L, -1);
		lua_getfield(L, index, "y"); point->y = luaL_checkinteger(L, -1);
		lua_pop(L, 2);
	}
	//-------------------------------------------------------------------------
	// ● create_point
	//-------------------------------------------------------------------------
	void create_point(lua_State* L, const SDL_Point* point) {
		lua_createtable(L, 0, 2);
		lua_pushnumber(L, point->x); lua_setfield(L, -2, "x");
		lua_pushnumber(L, point->y); lua_setfield(L, -2, "y");
	}
	//-------------------------------------------------------------------------
	// ● point_in_rect
	//-------------------------------------------------------------------------
	int point_in_rect(lua_State* L) {
		SDL_Point point;
		SDL_Rect rect;
		check_point(L, 1, &point);
		check_rect(L, 2, &rect);
		lua_pushboolean(L, SDL_PointInRect(&point, &rect));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"point_in_rect", point_in_rect},
			//{"has_intersection", has_intersection},
			//{"rect_empty", rect_empty},
			{NULL, NULL}
		};
		luaL_register(L, "Rect", reg);
		lua_pop(L, 1);
	}
}
