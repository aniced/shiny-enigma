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
	int key_states[SDL_NUM_SCANCODES] = {0};
	bool text_composition, text_input;
	char text_text[32];
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		// reduce all key states to 0/1
		for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
			key_states[i] = (bool) key_states[i];
		}
		text_input = false;

		SDL_Event e;
		while (SDL_PollEvent(&e)) switch (e.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_KEYDOWN:
			if (e.key.repeat) {
				key_states[e.key.keysym.scancode] = 2;
			} else {
				key_states[e.key.keysym.scancode] = 3;
			}
			break;
		case SDL_KEYUP:
			key_states[e.key.keysym.scancode] = 0;
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
		if (k < 0 || k >= ARRAY_SIZE(key_states)) {
			return luaL_error(L, "invalid key number: %d", k);
		}
		return k;
	}
	//-------------------------------------------------------------------------
	// ● key_pressed(scancode)
	//-------------------------------------------------------------------------
	int key_pressed(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, key_states[k]);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● key_triggered(scancode)
	//-------------------------------------------------------------------------
	int key_triggered(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, key_states[k] == 3);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● key_repeated(scancode)
	//-------------------------------------------------------------------------
	int key_repeated(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, key_states[k] >= 2);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● key_get_mods()
	//-------------------------------------------------------------------------
	int key_get_mods(lua_State* L) {
		SDL_Keymod mods = SDL_GetModState();
		lua_createtable(L, 0, 4);
		lua_pushboolean(L, mods & KMOD_NUM   ); lua_setfield(L, -2, "num");
		lua_pushboolean(L, mods & KMOD_CAPS  ); lua_setfield(L, -2, "caps");
		lua_pushboolean(L, mods & KMOD_MODE  ); lua_setfield(L, -2, "altgr");
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
	// ● text_get_text()
	//-------------------------------------------------------------------------
	int text_get_text(lua_State* L) {
		if (text_input) {
			lua_pushstring(L, text_text);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● mouse_get_point()
	//-------------------------------------------------------------------------
	int mouse_get_point(lua_State* L) {
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
			{"key_pressed", key_pressed},
			{"key_triggered", key_triggered},
			{"key_repeated", key_repeated},
			{"key_get_mods", key_get_mods},
			{"text_start", text_start},
			{"text_stop", text_stop},
			{"text_set_rect", text_set_rect},
			{"text_get_text", text_get_text},
			{"mouse_get_point", mouse_get_point},
			{NULL, NULL}
		};
		luaL_register(L, "Input", reg);
		lua_pop(L, 1);
	}
}
