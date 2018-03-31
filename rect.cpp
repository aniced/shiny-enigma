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
	// ● rect_empty(rect)
	//-------------------------------------------------------------------------
	int rect_empty(lua_State* L) {
		SDL_Rect rect;
		check_rect(L, 1, &rect);
		lua_pushboolean(L, SDL_RectEmpty(&rect));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● point_in_rect(point, rect)
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
	// ● has_intersection(rect1, rect2)
	//-------------------------------------------------------------------------
	int has_intersection(lua_State* L) {
		SDL_Rect rect1, rect2;
		check_rect(L, 1, &rect1);
		check_rect(L, 2, &rect2);
		lua_pushboolean(L, SDL_HasIntersection(&rect1, &rect1));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● intersect_rect(rect1, rect2)
	//   nil will be returned if there's no intersection.
	//-------------------------------------------------------------------------
	int intersect_rect(lua_State* L) {
		SDL_Rect rect1, rect2, rect3;
		check_rect(L, 1, &rect1);
		check_rect(L, 2, &rect2);
		if (SDL_IntersectRect(&rect1, &rect2, &rect3)) {
			create_rect(L, &rect3);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"rect_empty", rect_empty},
			{"point_in_rect", point_in_rect},
			{"has_intersection", has_intersection},
			{"intersect_rect", intersect_rect},
			{NULL, NULL}
		};
		luaL_register(L, "Rect", reg);
		lua_pop(L, 1);
	}
}
