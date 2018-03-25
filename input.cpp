//=============================================================================
// ■ input.cpp
//-----------------------------------------------------------------------------
//   A Lua module for handling input states which are updated per frame.
//   All SDL events are dealt with here.
//   In the script, you have two ways to check the input:
//   - call Input.key_press and the like and
//   - go through the table Input.events, which is refilled every frame.
//=============================================================================

namespace Input {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	int key_states[SDL_NUM_SCANCODES] = {0};
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) switch (event.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_KEYDOWN:
			key_states[event.key.keysym.scancode]++;
			break;
		case SDL_KEYUP:
			key_states[event.key.keysym.scancode] = 0;
			break;
		}
	}
	//-------------------------------------------------------------------------
	// ● key_pressed()
	//-------------------------------------------------------------------------
	int key_pressed(lua_State* L) {
		int k = luaL_checkinteger(L, 1);
		if (k < 0 || k >= ARRAY_SIZE(key_states)) {
			return luaL_error(L, "invalid key number: %d", k);
		}
		lua_pushboolean(L, key_states[k]);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"key_pressed", key_pressed},
			{NULL, NULL}
		};
		luaL_register(L, "Input", reg);
		lua_pop(L, 1);
	}
}
