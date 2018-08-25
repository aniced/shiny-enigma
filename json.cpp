//=============================================================================
// ■ JSON
//-----------------------------------------------------------------------------
//   A Lua module for reading and writing JSON strings.
//   It operates on IOStream (SDL_RWops*) objects.
//=============================================================================

namespace JSON {
	//-------------------------------------------------------------------------
	// ● parse(s)
	//-------------------------------------------------------------------------
	int lua_parse(lua_State* L) {
		size_t size;
		const char* mem = lua_tolstring(L, 1, &size);
		SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
		ParseState state(L, rw);
		parse_value(&state);
		SDL_RWclose(rw);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● parse_file(filename)
	//-------------------------------------------------------------------------
	int lua_parse_file(lua_State* L) {
		SDL_RWops* rw = SDL_RWFromFile(luaL_checkstring(L, 1), "r");
		ParseState state(L, rw);
		parse_value(&state);
		SDL_RWclose(rw);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● Stringify the value on top of the stack recursively       [-1, +1, e]
	//-------------------------------------------------------------------------
	void stringify(lua_State* L) {
		int index = lua_gettop(L);
		switch (lua_type(L, index)) {
		case LUA_TNONE:
			luaL_error(L, "trying to stringify nothing; perhaps a missing parameter?");
			break;
		case LUA_TNIL:
			lua_pushstring(L, "null");
			break;
		case LUA_TBOOLEAN:
			lua_pushstring(L, lua_toboolean(L, index) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			{
				#if LUA_FLOAT_TYPE != LUA_FLOAT_DOUBLE
					#error LUA_FLOAT_TYPE != LUA_FLOAT_DOUBLE
				#endif
				double number = lua_tonumber(L, index);
				if (isfinite(number)) {
					lua_pushfstring(L, "%f", number);
				} else {
					luaL_error(L, "trying to stringify NaN or Infinity");
				}
			}
			break;
		case LUA_TSTRING:
			{
				size_t len;
				const char* s = lua_tolstring(L, index, &len);
				luaL_Buffer B;
				luaL_buffinit(L, &B);
				luaL_addchar(&B, '\"');
				for (size_t i = 0; i < len; i++) switch (s[i]) {
				case '\"': case '\\':
					luaL_addchar(&B, '\\');
					luaL_addchar(&B, s[i]);
					break;
				case '\b': case '\t': case '\n': case '\f': case '\r':
					luaL_addchar(&B, '\\');
					luaL_addchar(&B, "btn fr"[s[i] - 8]);
					break;
				case 0: case 1: case 2: case 3:
				case 4: case 5: case 6: case 7:
				case 11: case 14: case 15:
				case 16: case 17: case 18: case 19:
				case 20: case 21: case 22: case 23:
				case 24: case 25: case 26: case 27:
				case 28: case 29: case 30: case 31:
					#if LUAL_BUFFERSIZE < 6
						#error LUAL_BUFFERSIZE too small
					#endif
					luaL_addsize(&B, sprintf(
						luaL_prepbuffer(&B),
						"\\u%04hhx",
						(unsigned char) s[i]
					));
					break;
				default:
					luaL_addchar(&B, s[i]);
				}
				luaL_addchar(&B, '\"');
				luaL_pushresult(&B);
			}
			break;
		case LUA_TTABLE:
			{
				size_t len = lua_objlen(L, index);
				// Note that the buffer occupies some of the stack,
				// which makes it a bit hard to do something else with the stack.
				luaL_Buffer B;
				luaL_buffinit(L, &B);
				if (len) {
					// table → array — save the array elements only
					luaL_addchar(&B, '[');
					// JSON arrays start from 0
					for (size_t i = 0; i <= len; i++) {
						if (i) luaL_addstring(&B, ", ");
						lua_rawgeti(L, index, i);
						stringify(L);
						luaL_addvalue(&B);
					}
					luaL_addchar(&B, ']');
				} else {
					// table → object — do a table traversal
					luaL_addchar(&B, '{');
					lua_pushnil(L);
					bool first = true;
					while (lua_next(L, index)) {
						// the key must be a string to be saved
						if (!lua_isstring(L, -2)) {
							lua_pop(L, 1);
							continue;
						}
						// pop the key and value to expose the buffer to the top
						int value = luaL_ref(L, LUA_REGISTRYINDEX);
						int key = luaL_ref(L, LUA_REGISTRYINDEX);
						// add a comma if necessary
						if (first) {
							first = false;
						} else {
							luaL_addstring(&B, ", ");
						}
						// stringify key
						lua_rawgeti(L, LUA_REGISTRYINDEX, key);
						stringify(L);
						luaL_addvalue(&B);
						luaL_addstring(&B, ": ");
						// stringify value
						lua_rawgeti(L, LUA_REGISTRYINDEX, value);
						stringify(L);
						luaL_addvalue(&B);
						// push the key back to be reused by lua_next
						lua_rawgeti(L, LUA_REGISTRYINDEX, key);
						// free the references
						luaL_unref(L, LUA_REGISTRYINDEX, key);
						luaL_unref(L, LUA_REGISTRYINDEX, value);
					}
					luaL_addchar(&B, '}');
				}
				luaL_pushresult(&B);
			}
			break;
		default:
			luaL_error(L, "unsupported type to stringify");
		}
		lua_remove(L, index);
	}
	//-------------------------------------------------------------------------
	// ● stringify(table)
	//-------------------------------------------------------------------------
	int lua_stringify(lua_State* L) {
		stringify(L);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"parse", lua_parse},
			{"parse_file", lua_parse_file},
			{"stringify", lua_stringify},
			{NULL, NULL}
		};
		luaL_register(L, "JSON", reg);
		lua_pop(L, 1);
	}
}
