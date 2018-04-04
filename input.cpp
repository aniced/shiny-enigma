//=============================================================================
// ■ input.cpp
//-----------------------------------------------------------------------------
//   A Lua module for handling input states which are updated per frame.
//   All SDL events are dealt with here.
//=============================================================================

namespace Input {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	// 0 = released; 1 = pressed; 2 = repeated; 3 = triggered
	int states[SDL_NUM_SCANCODES] = {0};
	bool text_composition, text_input;
	char text_text[32];
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		// reduce all key states to 0/1
		for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
			states[i] = (bool) states[i];
		}
		text_input = false;

		SDL_Event e;
		while (SDL_PollEvent(&e)) switch (e.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_KEYDOWN:
			if (e.key.repeat) {
				states[e.key.keysym.scancode] = 2;
			} else {
				states[e.key.keysym.scancode] = 3;
			}
			break;
		case SDL_KEYUP:
			states[e.key.keysym.scancode] = 0;
			break;
		case SDL_TEXTINPUT:
			text_input = true;
			memcpy(text_text, e.text.text, sizeof(text_text));
			break;
		}
	}
	//-------------------------------------------------------------------------
	// ● check_key
	//-------------------------------------------------------------------------
	int check_key(lua_State* L, int index) {
		int k = luaL_checkinteger(L, index);
		if (k < 0 || k >= ARRAY_SIZE(states)) {
			return luaL_error(L, "invalid key number: %d", k);
		}
		return k;
	}
	//-------------------------------------------------------------------------
	// ● pressed(scancode)
	//-------------------------------------------------------------------------
	int pressed(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k]);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● triggered(scancode)
	//-------------------------------------------------------------------------
	int triggered(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k] == 3);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● repeated(scancode)
	//-------------------------------------------------------------------------
	int repeated(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k] >= 2);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● mods()
	//-------------------------------------------------------------------------
	int mods(lua_State* L) {
		SDL_Keymod mods = SDL_GetModState();
		lua_createtable(L, 0, 15);
		#define _(kmod, field) \
			lua_pushboolean(L, mods & kmod); lua_setfield(L, -2, field)
		_(KMOD_SHIFT, "shift");
		_(KMOD_LSHIFT, "lshift");
		_(KMOD_RSHIFT, "rshift");
		_(KMOD_CTRL, "ctrl");
		_(KMOD_LCTRL, "lctrl");
		_(KMOD_RCTRL, "rctrl");
		_(KMOD_ALT, "alt");
		_(KMOD_LALT, "lalt");
		_(KMOD_RALT, "ralt");
		_(KMOD_GUI, "gui");
		_(KMOD_LGUI, "lgui");
		_(KMOD_RGUI, "rgui");
		_(KMOD_NUM, "num");
		_(KMOD_CAPS, "caps");
		_(KMOD_MODE, "altgr");
		#undef _
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● text_start()
	//-------------------------------------------------------------------------
	int text_start(lua_State* L) {
		SDL_StartTextInput();
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text_stop()
	//-------------------------------------------------------------------------
	int text_stop(lua_State* L) {
		SDL_StopTextInput();
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text_set_rect()
	//-------------------------------------------------------------------------
	int text_set_rect(lua_State* L) {
		SDL_Rect rect;
		Rect::to_rect(L, 1, &rect);
		SDL_SetTextInputRect(&rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text()
	//-------------------------------------------------------------------------
	int text(lua_State* L) {
		if (text_input) {
			lua_pushstring(L, text_text);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● mouse()
	//-------------------------------------------------------------------------
	int mouse(lua_State* L) {
		SDL_Point point;
		SDL_GetMouseState(&point.x, &point.y);
		Rect::create_point(L, &point);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"pressed", pressed},
			{"triggered", triggered},
			{"repeated", repeated},
			{"mods", mods},
			{"text", text},
			{"text_start", text_start},
			{"text_stop", text_stop},
			{"text_set_rect", text_set_rect},
			{"mouse", mouse},
			{NULL, NULL}
		};
		luaL_register(L, "Input", reg);
		lua_pop(L, 1);
	}
}
