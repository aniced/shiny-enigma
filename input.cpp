//=============================================================================
// ■ Input
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
	// see action()
	bool state_action;
	bool text_composition, text_input;
	char text_text[32];
	bool pause_on_blur = true;
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		// reduce all key states to 0/1
		for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
			states[i] = (bool) states[i];
		}
		state_action = false;
		text_input = false;

		SDL_Event e;
		while (SDL_PollEvent(&e)) switch (e.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				if (pause_on_blur) {
					do {
						SDL_WaitEvent(&e);
					} while (e.type != SDL_WINDOWEVENT
						|| e.window.event != SDL_WINDOWEVENT_FOCUS_GAINED);
				}
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED: {
				int w, h;
				SDL_GetRendererOutputSize($renderer, &w, &h);
				Graphics::set_size(w, h);
				break;
			}
			}
			break;
		case SDL_KEYDOWN:
			state_action = true;
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
			state_action = true;
			text_input = true;
			memcpy(text_text, e.text.text, sizeof(text_text));
			break;
		case SDL_MOUSEBUTTONDOWN:
			state_action = true;
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		}
	}
	//-------------------------------------------------------------------------
	// ● action()
	//   true is returned if the user performed an action.
	//   When this function returns true, animations should stop.
	//-------------------------------------------------------------------------
	int lua_action(lua_State* L) {
		lua_pushboolean(L, state_action);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● check_key
	//-------------------------------------------------------------------------
	int check_key(lua_State* L, int index) {
		int k = luaL_checkinteger(L, index);
		if (k < 0 || k >= (int) ARRAY_SIZE(states)) {
			return luaL_error(L, "invalid key number: %d", k);
		}
		return k;
	}
	//-------------------------------------------------------------------------
	// ● pressed(scancode)
	//-------------------------------------------------------------------------
	int lua_pressed(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k]);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● triggered(scancode)
	//-------------------------------------------------------------------------
	int lua_triggered(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k] == 3);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● repeated(scancode)
	//-------------------------------------------------------------------------
	int lua_repeated(lua_State* L) {
		int k = check_key(L, 1);
		lua_pushboolean(L, states[k] >= 2);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● mods()
	//-------------------------------------------------------------------------
	int lua_mods(lua_State* L) {
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
	int lua_text_start(lua_State* L) {
		SDL_StartTextInput();
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text_stop()
	//-------------------------------------------------------------------------
	int lua_text_stop(lua_State* L) {
		SDL_StopTextInput();
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text_set_rect()
	//-------------------------------------------------------------------------
	int lua_text_set_rect(lua_State* L) {
		SDL_Rect rect;
		Geometry::to_rect(L, 1, &rect);
		SDL_SetTextInputRect(&rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● text()
	//-------------------------------------------------------------------------
	int lua_text(lua_State* L) {
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
	int lua_mouse(lua_State* L) {
		SDL_Point point;
		SDL_GetMouseState(&point.x, &point.y);
		Geometry::create_point(L, &point);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● set_pause_on_blur(enabled)
	//-------------------------------------------------------------------------
	int lua_set_pause_on_blur(lua_State* L) {
		luaL_checkany(L, 1);
		pause_on_blur = lua_toboolean(L, 1);
		if (!(SDL_GetWindowFlags($window) & SDL_WINDOW_INPUT_FOCUS)) {
			SDL_Event e;
			e.type = SDL_WINDOWEVENT;
			e.window.windowID = SDL_GetWindowID($window);
			e.window.event = SDL_WINDOWEVENT_FOCUS_GAINED;
			SDL_PushEvent(&e);
		}
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"action", lua_action},
			{"pressed", lua_pressed},
			{"triggered", lua_triggered},
			{"repeated", lua_repeated},
			{"mods", lua_mods},
			{"text", lua_text},
			{"text_start", lua_text_start},
			{"text_stop", lua_text_stop},
			{"text_set_rect", lua_text_set_rect},
			{"mouse", lua_mouse},
			{"set_pause_on_blur", lua_set_pause_on_blur},
			{NULL, NULL}
		};
		luaL_register(L, "Input", reg);
		lua_pop(L, 1);
	}
}
