//=============================================================================
// ■ JSON::ParseState
//-----------------------------------------------------------------------------
//   A C++ class that handles JSON parsing.
//=============================================================================

namespace JSON { class ParseState {
public:
	//-------------------------------------------------------------------------
	// ● Declarations
	//-------------------------------------------------------------------------
	lua_State* L;
	SDL_RWops* rw;
	// Logically, the parser looks 1 character ahead.
	// Actually, we read one more character in the RWops
	// than we logically read and buffer it in next_char.
	char next_char;
	//-------------------------------------------------------------------------
	// ● (Logically) read the next character
	//-------------------------------------------------------------------------
	char read_char() {
		char tmp = next_char;
		next_char = (char) (unsigned char) SDL_ReadU8(rw);
		return tmp;
	}
	void read_char(char expected) {
		char actual = read_char();
		if (expected != actual) {
			luaL_error(L, "%c expected (got %c)", expected, actual);
		}
	}
	//-------------------------------------------------------------------------
	// ● Constructor
	//-------------------------------------------------------------------------
	ParseState(lua_State* LL, SDL_RWops* rwrw) {
		L = LL;
		rw = rwrw;
		next_char = 0;
		read_char();
	}
	//-------------------------------------------------------------------------
	// ● Skip the whitespace in front
	//-------------------------------------------------------------------------
	void skip_whitespace() {
		while (strchr(" \n\t\r", next_char)) read_char();
	}
	//-------------------------------------------------------------------------
	// ● Parse a literal true, false or null
	//-------------------------------------------------------------------------
	void parse_literal() {
		skip_whitespace();
		switch(read_char()) {
		case 't':
			read_char('r');
			read_char('u');
			read_char('e');
			lua_pushboolean(L, true);
			break;
		case 'f':
			read_char('a');
			read_char('l');
			read_char('s');
			read_char('e');
			lua_pushboolean(L, false);
			break;
		case 'n':
			read_char('u');
			read_char('l');
			read_char('l');
			lua_pushnil(L);
			break;
		default:
			luaL_error(L, "invalid literal");
		}
	}
	//-------------------------------------------------------------------------
	// ● Parse a number
	//-------------------------------------------------------------------------
	void parse_number() {
		skip_whitespace();
		// extract the number part first
		char s[100];
		char* p = s;
		while (strchr("0123456789+-.eE", next_char)) {
			*p = read_char();
			p++;
			if (p >= s + sizeof(s) - 1) {
				luaL_error(L, "number too long");
			}
		}
		*p = 0;
		// Let the C library do the magic.
		// Not strictly conforming, but it works.
		double number = strtod(s, &p);
		if (*p) {
			luaL_error(L, "bad conversion to number");
		}
		lua_pushnumber(L, number);
	}
	//-------------------------------------------------------------------------
	// ● Parse a string
	//-------------------------------------------------------------------------
	void parse_string() {
		skip_whitespace();
		read_char('\"');
		char c;
		luaL_Buffer B;
		luaL_buffinit(L, &B);
		while ((c = read_char()) != '\"') {
			if (c == '\\') {
				c = read_char();
				switch (c) {
				case 'b': case 't': case 'n': case 'f': case 'r':
					luaL_addchar(&B, " \b \f   \n \r\t"[(c >> 1) & 15]);
					break;
				case '\"': case '\\': case '/':
					luaL_addchar(&B, c);
					break;
				case 'u':
					{
						char32_t codepoint;
						char s[9];
						s[0] = read_char();
						s[1] = read_char();
						s[2] = read_char();
						s[3] = read_char();
						s[4] = 0;
						if ((s[0] == 'D' || s[0] == 'd') && strchr("89abAB", s[1])) {
							if (read_char() != '\\' || read_char() != 'u') {
								luaL_error(L, "incomplete surrogate pair");
							}
							s[4] = read_char();
							s[5] = read_char();
							s[6] = read_char();
							s[7] = read_char();
							s[8] = 0;
						}
						sscanf(s, "%" SCNxLEAST32, (uint_least32_t*) &codepoint);
						if (s[4]) {
							codepoint = (codepoint & 0x3ff) | (((codepoint >> 16) & 0x3ff) << 10);
							codepoint += 0x10000;
						}
						UTF8::add_encode(&B, codepoint);
					}
					break;
				default:
					luaL_error(L, "undefined escape sequence");
				}
			} else {
				luaL_addchar(&B, c);
			}
		}
		luaL_pushresult(&B);
	}
	//-------------------------------------------------------------------------
	// ● Parse an array
	//-------------------------------------------------------------------------
	void parse_array() {
		skip_whitespace();
		read_char('[');
		lua_newtable(L);
		for (int i = 0; skip_whitespace(), next_char != ']'; i++) {
			if (i) read_char(',');
			parse_value();
			lua_rawseti(L, -2, i);
		}
		read_char(']');
	}
	//-------------------------------------------------------------------------
	// ● Parse an object
	//-------------------------------------------------------------------------
	void parse_object() {
		skip_whitespace();
		read_char('{');
		lua_newtable(L);
		bool first = true;
		for (;;) {
			skip_whitespace();
			if (next_char == '}') break;
			if (first) {
				first = false;
			} else {
				read_char(',');
			}
			parse_string();
			skip_whitespace();
			read_char(':');
			parse_value();
			lua_rawset(L, -3);
		}
		read_char('}');
	}
	//-------------------------------------------------------------------------
	// ● Parse and push the next JSON value in the specified RWops [-0, +1, e]
	//-------------------------------------------------------------------------
	void parse_value() {
		skip_whitespace();
		switch (next_char) {
		case 't': case 'f': case 'n':
			parse_literal();
			break;
		case '{':
			parse_object();
			break;
		case '[':
			parse_array();
			break;
		case '\"':
			parse_string();
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '-':
			parse_number();
			break;
		// more extensive error messages
		case '+':
			luaL_error(L, "a number cannot start with a plus sign");
			break;
		case '.':
			luaL_error(L, "one zero before the decimal point is mandatory");
			break;
		default:
			luaL_error(L, "unexpected character");
		}
	}
}; }
