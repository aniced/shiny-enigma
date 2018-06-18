//=============================================================================
// ■ UTF8 / utf8
//-----------------------------------------------------------------------------
//   A Lua module for UTF-8 encoding and decoding.
//=============================================================================

namespace UTF8 {
	//-------------------------------------------------------------------------
	// ● Module declarations
	//-------------------------------------------------------------------------
	const char* charpattern = "[%z\1-\x7f\xc2-\xf4][\x80-\xbf]*";
	//-------------------------------------------------------------------------
	// ● add_encode
	//-------------------------------------------------------------------------
	void add_encode(luaL_Buffer* B, char32_t ch) {
		if (ch < 0x80) {
			luaL_addchar(B, ch);
		} else {
			uint8_t s[5] = {0, 0, 0, 0, 0};
			if (ch < 0x800) {
				s[0] = 0b11000000 | (ch >> 6);
				s[1] = 0b10000000 | (ch & 0b00111111);
			} else if (ch < 0x10000) {
				s[0] = 0b11100000 | (ch >> 12);
				s[1] = 0b10000000 | ((ch >> 6) & 0b00111111);
				s[2] = 0b10000000 | (ch & 0b00111111);
			} else if (ch < 0x110000) {
				s[0] = 0b11110000 | (ch >> 18);
				s[1] = 0b10000000 | ((ch >> 12) & 0b00111111);
				s[2] = 0b10000000 | ((ch >> 6) & 0b00111111);
				s[3] = 0b10000000 | (ch & 0b00111111);
			} else {
				luaL_error(L,
					"invalid character %" PRIu32 " (0x" PRIX32 ")",
					ch, ch
				);
			}
			luaL_addstring(B, (const char*) s);
		}
	}
	//-------------------------------------------------------------------------
	// ● decode
	//   seq: the string of which the first codepoint is desired
	//   mov: a pointer to a pointer that should be increased
	//        to track the next character
	//-------------------------------------------------------------------------
	char32_t decode(const char* seq, const char** mov = NULL) {
		uint8_t* s = (uint8_t*) seq;
		if (*s < 0x80) {
			if (mov) (*mov)++;
			return *s;
		} else {
			char32_t ch;
			if (*s < 0b11100000) {
				ch = (char32_t) (*s++ & 0b00011111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x80) {
					luaL_error(L, "overlong UTF-8 sequence for %d", ch);
				}
				if (mov) (*mov) += 2;
			} else if (*s < 0b11110000) {
				ch = (char32_t) (*s++ & 0b00001111) << 12;
				ch |= (char32_t) (*s++ & 0b00111111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x800) {
					luaL_error(L, "overlong UTF-8 sequence for %d", ch);
				}
				if (mov) (*mov) += 3;
			} else if (*s < 0b11111000) {
				ch = (char32_t) (*s++ & 0b00000111) << 18;
				ch |= (char32_t) (*s++ & 0b00111111) << 12;
				ch |= (char32_t) (*s++ & 0b00111111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x10000) {
					luaL_error(L, "overlong UTF-8 sequence for %d", ch);
				} else if (ch > 0x10ffff) {
					luaL_error(L, "invalid Unicode codepoint %d", ch);
				}
				if (mov) (*mov) += 4;
			} else {
				luaL_error(L, "invalid UTF-8 sequence prefix %d", *s);
			}
			return ch;
		}
	}
	//-------------------------------------------------------------------------
	// ● is_continuation
	//-------------------------------------------------------------------------
	inline bool is_continuation(char c) {
		return ((uint8_t) c >> 6) == 2;
	}
	//-------------------------------------------------------------------------
	// ● skip_char
	//-------------------------------------------------------------------------
	const char* skip_char(const char* s) {
		s++;
		while (is_continuation(*s)) s++;
		return s;
	}
	//-------------------------------------------------------------------------
	// ● char(...)
	//-------------------------------------------------------------------------
	int lua_char(lua_State* L) {
		int n = lua_gettop(L);
		luaL_Buffer b;
    luaL_buffinit(L, &b);
    for (int i = 1; i <= n; i++) {
			add_encode(&b, luaL_checkint(L, i));
    }
    luaL_pushresult(&b);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● codes(s)
	//-------------------------------------------------------------------------
	int lua_codes_iterator(lua_State* L) {
  	size_t len;
  	const char* s = luaL_checklstring(L, 1, &len);
		const char* p;
		if (lua_isnil(L, 2)) {
			p = s;
		} else {
			p = skip_char(s + luaL_checkint(L, 2) - 1);
		}
		if (p < s + len) {
			lua_pushnumber(L, p - s + 1);
			lua_pushnumber(L, decode(p));
			return 2;
		} else {
			return 0;
		}
	}
	int lua_codes(lua_State* L) {
		luaL_checktype(L, 1, LUA_TSTRING);
		lua_pushcfunction(L, codes_iterator);
		lua_pushvalue(L, 1);
		lua_pushnil(L);
		return 3;
	}
	//-------------------------------------------------------------------------
	// ● codepoint(s, i = 1, j = i)
	//   Note that this function doesn't throw errors
	//   if i or j is out of range.
	//-------------------------------------------------------------------------
	int lua_codepoint(lua_State* L) {
		size_t len1;
		const char* s = luaL_checklstring(L, 1, &len1);
		int len = len1;
		int i = Util::translate_pos(L, 1, luaL_optint(L, 2, 1));
		if (i < 1) i = 1;
		if (i > len) return 0;
		int j = Util::translate_pos(L, 1, luaL_optint(L, 3, i));
		if (j < 1) return 0;
		if (j > len) j = len;
		const char* p = s + i - 1;
		int count = 0;
		while (p - s < j) {
			lua_pushnumber(L, decode(p, &p));
			count++;
		}
		return count;
	}
	//-------------------------------------------------------------------------
	// ● len(s, i = 1, j = -1)
	//-------------------------------------------------------------------------
	int lua_len(lua_State* L) {
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● offset(s, n, i = (n >= 0 and 1 or #n + 1))
	//-------------------------------------------------------------------------
	int lua_offset(lua_State* L) {
		int len = lua_objlen(L, 1);
		const char* s = luaL_checklstring(L, 1, NULL);
		int n = luaL_checkint(L, 2);
		int i = luaL_optint(L, 3, n >= 0 ? 1 : len + 1);
		i = Util::translate_pos(L, 1, i);
		const char* p = s + i - 1;
		if (n == 0) {
			while (is_continuation(*p)) p--;
		} else if (n > 0) {
			n--;
			while (n) {
				p = skip_char(p);
				n--;
			}
		} else {
			while (n) {
				do {
					p--;
				} while (is_continuation(*p));
				n++;
			}
		}
		lua_pushnumber(L, p - s + 1);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"char", lua_char},
			{"charpattern", NULL},
			{"codes", lua_codes},
			{"codepoint", lua_codepoint},
			//{"len", lua_len},
			{"offset", lua_offset},
			{NULL, NULL}
		};
		luaL_register(L, "utf8", reg);
		lua_pushstring(L, charpattern);
		lua_setfield(L, -2, "charpattern");
		lua_pop(L, 1);
	}
}
