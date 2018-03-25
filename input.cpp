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
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		// reduce all key states to 0/1
		for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
			key_states[i] = (bool) key_states[i];
		}

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
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"key_pressed", key_pressed},
			{"key_triggered", key_triggered},
			{"key_repeated", key_repeated},
			{"text_start", text_start},
			{"text_stop", text_stop},
			{NULL, NULL}
		};
		luaL_register(L, "Input", reg);
		lua_pop(L, 1);
	}
}
