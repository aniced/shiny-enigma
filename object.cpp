//=============================================================================
// ■ Object
//-----------------------------------------------------------------------------
//   The super class of all Lua objects.
//=============================================================================

namespace Object {
	//-------------------------------------------------------------------------
	// ● new()
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		lua_createtable(L, 0, 0);
		luaL_getmetatable(L, "Ｏｂｊｅｃｔ");
		lua_setmetatable(L, -2);
		//const luaL_reg reg[] = {
		//{NULL, NULL}
		//};
		//Util::instance_register(L, reg);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● __index(table, key)
	//-------------------------------------------------------------------------
	int lua_meta_index(lua_State* L) {
		// method_name = "get_" .. key
		lua_pushstring(L, "get_");
		lua_pushvalue(L, 2);
		lua_concat(L, 2);
		// method = table[method_name]
		lua_rawget(L, 1);
		// if method == nil then
		if (lua_isnil(L, -1)) {
			// return nil
			return 1;
		} else {
			// return method()
			lua_call(L, 0, LUA_MULTRET);
			return lua_gettop(L) - 2;
		}
	}
	//-------------------------------------------------------------------------
	// ● __newindex(table, key, value)
	//-------------------------------------------------------------------------
	int lua_meta_newindex(lua_State* L) {
		// method_name = "set_" .. key
		lua_pushstring(L, "set_");
		lua_pushvalue(L, 2);
		lua_concat(L, 2);
		// method = table[method_name]
		lua_rawget(L, 1);
		// if method == nil then
		if (lua_isnil(L, -1)) {
			// table[key] = value
			lua_pop(L, 1);
			lua_rawset(L, 1);
			// return
			return 0;
		} else {
			// return method(value)
			lua_pushvalue(L, 3);
			lua_call(L, 1, LUA_MULTRET);
			return lua_gettop(L) - 3;
		}
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		{
			const luaL_reg reg[] = {
				{"new", lua_new_instance},
				{NULL, NULL}
			};
			luaL_register(L, "Object", reg);
			lua_pop(L, 1);
		}
		{
			const luaL_reg reg[] = {
				{"__index", lua_meta_index},
				{"__newindex", lua_meta_newindex},
				{NULL, NULL}
			};
			luaL_newmetatable(L, "Ｏｂｊｅｃｔ");
			luaL_register(L, NULL, reg);
			lua_pop(L, 1);
		}
	}
}
