//=============================================================================
// ■ UTF8 / utf8
//-----------------------------------------------------------------------------
//   A Lua module for UTF-8 encoding and decoding.
//=============================================================================

namespace UTF8 {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	const char charpattern[] = {
		'[', 0, '-', 0x7f, (char) 0xc2, '-', (char) 0xf4, ']',
		'[', (char) 0x80, '-', (char) 0xbf, ']', '*'
	};
	//-------------------------------------------------------------------------
	// ● add_encode
	//-------------------------------------------------------------------------
	void add_encode(luaL_Buffer* B, uint32_t ch) {
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
	uint32_t decode(const char* seq, const char** mov = NULL) {
		uint8_t* s = (uint8_t*) seq;
		if (*s < 0x80) {
			if (mov) (*mov)++;
			return *s;
		} else {
			uint32_t ch;
			if (*s < 0b11100000) {
				ch = (uint32_t) (*s++ & 0b00011111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x80) {
					luaL_error(L, "overlong UTF-8 sequence for U+%04" PRIX32, ch);
				}
				if (mov) (*mov) += 2;
			} else if (*s < 0b11110000) {
				ch = (uint32_t) (*s++ & 0b00001111) << 12;
				ch = (uint32_t) (*s++ & 0b00111111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x800) {
					luaL_error(L, "overlong UTF-8 sequence for U+%04" PRIX32, ch);
				}
				if (mov) (*mov) += 3;
			} else if (*s < 0b11111000) {
				ch = (uint32_t) (*s++ & 0b00000111) << 18;
				ch = (uint32_t) (*s++ & 0b00111111) << 12;
				ch = (uint32_t) (*s++ & 0b00111111) << 6;
				ch |= (*s & 0b00111111);
				if (ch < 0x10000) {
					luaL_error(L, "overlong UTF-8 sequence for U+%04" PRIX32, ch);
				} else if (ch > 0x10ffff) {
					luaL_error(L, "invalid Unicode codepoint U+%" PRIX32, ch);
				}
				if (mov) (*mov) += 4;
			} else {
				luaL_error(L, "invalid UTF-8 sequence prefix 0x%" PRIX8, *s);
			}
			return ch;
		}
	}
	//-------------------------------------------------------------------------
	// ● skip_char
	//-------------------------------------------------------------------------
	const char* skip_char(const char* s) {
		s++;
		while ((*((const uint8_t*) s) >> 6) == 2) s++;
		return s;
	}
	//-------------------------------------------------------------------------
	// ● char(...)
	//-------------------------------------------------------------------------
	int _char(lua_State* L) {
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
	int codes_iterator(lua_State* L) {
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
	int codes(lua_State* L) {
		luaL_checktype(L, 1, LUA_TSTRING);
		lua_pushcfunction(L, codes_iterator);
		lua_pushvalue(L, 1);
		lua_pushnil(L);
		return 3;
	}
	//-------------------------------------------------------------------------
	// ●
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ●
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ●
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"char", _char},
			{"charpattern", NULL},
			{"codes", codes},
			//{"codepoint", codepoint},
			//{"len", len},
			//{"offset", offset},
			{NULL, NULL}
		};
		luaL_register(L, "utf8", reg);
		lua_pushlstring(L, charpattern, sizeof(charpattern));
		lua_setfield(L, -2, "charpattern");
		lua_pop(L, 1);
	}
}