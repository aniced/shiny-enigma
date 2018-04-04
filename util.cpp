//=============================================================================
// ■ Util
//-----------------------------------------------------------------------------
//   A module for dealing with random things in Lua.
//   As the number of the functions grows, they will eventually be moved
//   to a new file and have their own category.
//=============================================================================

namespace Util {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	const char* base_path;
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
	//-------------------------------------------------------------------------
	// ● rtp(filename)
	//-------------------------------------------------------------------------
	char* rtp(const char* path) {
		static char s[256];
		strcpy(s, base_path);
		strcat(s, path);
		return s;
	}
	int rtp_lua(lua_State* L) {
		const char* path = luaL_checkstring(L, 1);
		lua_pushstring(L, rtp(path));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● initialize rtp
	//-------------------------------------------------------------------------
	void init_rtp() {
		const char* p = base_path = SDL_GetBasePath();
		while (*p) {
			if (*p & (char) 0x80) {
				error("base path is not ASCII");
			}
			p++;
		}
	}
	//-------------------------------------------------------------------------
	// ● call a handler function
	//-------------------------------------------------------------------------
	void call_handler(const char* name) {
		// _G.on[name]()
		lua_getglobal(L, "on");
		lua_getfield(L, -1, name);
		lua_call(L, 0, 0);
	}
	//-------------------------------------------------------------------------
	// ● class.new closure
	//   upvalue 1 = class
	//   upvalue 2 = parent
	//-------------------------------------------------------------------------
	int new_instance(lua_State* L) {
		// local self = {}
		lua_newtable(L);
		// setmetatable(self, class)
		lua_pushvalue(L, lua_upvalueindex(1));
		lua_setmetatable(L, -2);
		// class.init(self)
		lua_getfield(L, lua_upvalueindex(1), "init");
		lua_pushvalue(L, -2);
		lua_call(L, 1, 0);
		// return self
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● class(parent = nil)
	//-------------------------------------------------------------------------
	int new_class(lua_State* L) {
		bool has_parent = !lua_isnoneornil(L, 1);
		// t = {}
		lua_createtable(L, 0, 2);
		// t.__index = t
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		// if arg[1] then
		if (has_parent) {
			// setmetatable(t, arg[1])
			lua_pushvalue(L, 1);
			lua_setmetatable(L, -2);
		}
		// t.new = …
		lua_pushvalue(L, -1);
		if (has_parent) {
			lua_pushvalue(L, 1);
		} else {
			lua_pushnil(L);
		}
		lua_pushcclosure(L, new_instance, 2);
		lua_setfield(L, -2, "new");
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//   base_path is set independently.
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"class", new_class},
			{"rtp", rtp_lua},
			{NULL, NULL}
		};
		luaL_register(L, "Util", reg);
		lua_pop(L, 1);
	}
}
