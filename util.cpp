//=============================================================================
// ■ Util
//-----------------------------------------------------------------------------
//   A Lua module for dealing with random things.
//   As the number of the functions grows, they will eventually be moved
//   to a new file and have their own category.
//=============================================================================

namespace Util {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	const char* base_path;
	//-------------------------------------------------------------------------
	// ● check_boolean
	//-------------------------------------------------------------------------
	bool check_boolean(lua_State* L, int index) {
		luaL_argcheck(L, lua_isboolean(L, index), index, "boolean expected");
		return lua_toboolean(L, index);
	}
	//-------------------------------------------------------------------------
	// ● clamp
	//-------------------------------------------------------------------------
	template <class T> inline T clamp(T x, T min, T max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	//-------------------------------------------------------------------------
	// ● translate_pos
	//    index : the string
	//-------------------------------------------------------------------------
	int translate_pos(lua_State* L, int index, int pos) {
		if (pos >= 0) {
			return pos;
		} else {
			int len = lua_objlen(L, index);
			return len + pos + 1;
		}
	}
	//-------------------------------------------------------------------------
	// ● check_pos
	//   This can throw an error.
	//-------------------------------------------------------------------------
	int check_pos(lua_State* L, int index, int pos) {
		int len = lua_objlen(L, index);
		int p = pos >= 0 ? pos : len + pos + 1;
		if (p < 1 || p > len) luaL_error(L, "position %d out of string", pos);
		return p;
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
	//-------------------------------------------------------------------------
	// ● rtp(filename)
	//-------------------------------------------------------------------------
	char* rtp(const char* path) {
		static char s[256];
		strcpy(s, base_path);
		strcat(s, path);
		return s;
	}
	int lua_rtp(lua_State* L) {
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
		// local f = _G.on[name]
		lua_getglobal(L, "on");
		lua_getfield(L, -1, name);
		// if f then f() end
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
		} else {
			lua_call(L, 0, 0);
		}
	}
	//-------------------------------------------------------------------------
	// ● register closures with an upvalue of self into the table on top
	//-------------------------------------------------------------------------
	void instance_register(lua_State* L, const luaL_Reg* l) {
		while (l->name || l->func) {
			if (l->name && l->func) {
				lua_pushvalue(L, -1);
				lua_pushcclosure(L, l->func, 1);
				lua_setfield(L, -2, l->name);
			}
			l++;
		}
	}
	//-------------------------------------------------------------------------
	// ● shallow_copy(obj)
	//-------------------------------------------------------------------------
	int lua_shallow_copy(lua_State* L) {
		lua_newtable(L);
		lua_pushnil(L);
		while (lua_next(L, 1)) {
			lua_pushvalue(L, 3);
			lua_pushvalue(L, 4);
			lua_settable(L, 2);
			lua_settop(L, 3);
		}
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● os_encoding_to_utf8
	//-------------------------------------------------------------------------
	char* os_encoding_to_utf8(const char* os_string) {
		static char r[1024]; // result
		#ifdef __WINDOWS__
			static wchar_t unicode[1024];
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				os_string, -1,
				unicode, ARRAY_SIZE(unicode)
			);
			WideCharToMultiByte(
				CP_UTF8,
				0,
				unicode, ARRAY_SIZE(unicode),
				r, ARRAY_SIZE(r),
				NULL, NULL
			);
			return r;
		#else
			return os_string;
		#endif
	}
	//-------------------------------------------------------------------------
	// ● init
	//   base_path is set independently.
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"rtp", lua_rtp},
			{"shallow_copy", lua_shallow_copy},
			{NULL, NULL}
		};
		luaL_register(L, "Util", reg);
		lua_pop(L, 1);
	}
}
