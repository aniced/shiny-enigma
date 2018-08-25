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
		if (read_char() != expected) {
			luaL_error(L, "%c expected", expected);
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
			if (read_char() == 'r') {
				if (read_char() == 'u') {
					if (read_char() == 'e') {
						lua_pushboolean(L, true);
					} else goto on_error;
				} else goto on_error;
			} else goto on_error;
			break;
		case 'f':
			if (read_char() == 'a') {
				if (read_char() == 'l') {
					if (read_char() == 's') {
						if (read_char() == 'e') {
							lua_pushboolean(L, false);
						} else goto on_error;
					} else goto on_error;
				} else goto on_error;
			} else goto on_error;
			break;
		case 'n':
			if (read_char() == 'u') {
				if (read_char() == 'l') {
					if (read_char() == 'l') {
						lua_pushnil(L);
					} else goto on_error;
				} else goto on_error;
			} else goto on_error;
			break;
		default:
			goto on_error;
		}
		return;
	on_error:
		luaL_error(L, "invalid literal");
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
				case 'b': luaL_addchar(&B, '\b'); break;
				case 't': luaL_addchar(&B, '\t'); break;
				case 'n': luaL_addchar(&B, '\n'); break;
				case 'f': luaL_addchar(&B, '\f'); break;
				case 'r': luaL_addchar(&B, '\r'); break;
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
						sscanf(s, "%" SCNxLEAST32, (uint_least32_t*) &codepoint);
						if ((codepoint & 0xd800) == 0xd800) {
							if (read_char() != '\\' || read_char() != 'u') {
								luaL_error(L, "incomplete surrogate pair");
							}
							s[4] = read_char();
							s[5] = read_char();
							s[6] = read_char();
							s[7] = read_char();
							s[8] = 0;
							sscanf(s, "%" SCNxLEAST32, (uint_least32_t*) &codepoint);
							codepoint = (codepoint & 0x3ff) | ((codepoint >> 16) & 0x3ff);
						}
						luaL_addchar(&B, );
					}
					break;
				default:
					luaL_error(L, "undefined escape sequence");
				}
			} else {
				luaL_addchar(&B, c);
			}
		}
	}
	//-------------------------------------------------------------------------
	// ● Parse an array
	//-------------------------------------------------------------------------
	void parse_array() {
		skip_whitespace();
	}
	//-------------------------------------------------------------------------
	// ● Parse an object
	//-------------------------------------------------------------------------
	void parse_object() {
		skip_whitespace();
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
