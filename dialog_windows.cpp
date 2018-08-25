//=============================================================================
// ■ Dialog
//-----------------------------------------------------------------------------
//   A Lua module providing access to native or emulated modal dialogs.
//   A few optional parameters are common among the preset dialogs:
//       context: "error", "warning", "information" (the default) or ""
//           "info" shows a question mark in dialogs that require the user
//           to input, or an “i” icon in dialogs that do not.
//=============================================================================

namespace Dialog {
	//-------------------------------------------------------------------------
	// ● check_context
	//-------------------------------------------------------------------------
	Uint32 check_context(lua_State* L, int narg) {
		const char* s = luaL_optstring(L, narg, NULL);
		if (!s || strcmp(s, "information") == 0) {
			return SDL_MESSAGEBOX_INFORMATION;
		} else if (strcmp(s, "warning") == 0) {
			return SDL_MESSAGEBOX_WARNING;
		} else if (strcmp(s, "error") == 0) {
			return SDL_MESSAGEBOX_ERROR;
		} else {
			return 0;
		}
	}
	//-------------------------------------------------------------------------
	// ● check_context_windows
	//-------------------------------------------------------------------------
	UINT check_context_windows(lua_State* L, int narg, bool question) {
		switch (check_context(L, narg)) {
		case SDL_MESSAGEBOX_INFORMATION:
			return question ? MB_ICONQUESTION : MB_ICONINFORMATION;
		case SDL_MESSAGEBOX_WARNING:
			return MB_ICONWARNING;
		case SDL_MESSAGEBOX_ERROR:
			return MB_ICONERROR;
		default:
			return 0;
		}
	}
	//-------------------------------------------------------------------------
	// ● show_message(message[, context])
	//-------------------------------------------------------------------------
	int lua_show_message(lua_State* L) {
		SDL_SysWMinfo wm_info;
		SDL_VERSION(&wm_info.version);
		SDL_GetWindowWMInfo($window, &wm_info);
		MessageBoxA(
			wm_info.info.win.window,
			Util::utf8_to_os_encoding(luaL_checkstring(L, 1)),
			APPLICATION_TITLE,
			check_context_windows(L, 2, false)
		);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● show_choice(message, buttons[, context])
	//    buttons : an array of strings
	//-------------------------------------------------------------------------
	int lua_show_choice(lua_State* L) {
		SDL_MessageBoxData data;
		data.flags = check_context(L, 3);
		data.window = $window;
		data.title = APPLICATION_TITLE;
		data.message = luaL_checkstring(L, 1);
		data.numbuttons = lua_objlen(L, 2);
		SDL_MessageBoxButtonData buttons[data.numbuttons];
		data.buttons = (const SDL_MessageBoxButtonData*) &buttons;
		for (int i = 1; i <= data.numbuttons; i++) {
			lua_rawgeti(L, 2, i);
			buttons[i - 1].flags = 0;
			buttons[i - 1].buttonid = i;
			buttons[i - 1].text = lua_tostring(L, -1);
		}
		int buttonid;
		if (!SDL_ShowMessageBox(&data, &buttonid)) {
			Util::sdlerror(L, "SDL_ShowMessageBox() < 0");
		}
		lua_pushnumber(L, buttonid);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● show_input(prompt, default = ""[, context])
	//-------------------------------------------------------------------------
	int lua_show_input(lua_State* L) {
		return 3;
	}
	//-------------------------------------------------------------------------
	// ● show_
	//-------------------------------------------------------------------------
	int lua_show_(lua_State* L) {
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"show_message", lua_show_message},
			{"show_choice", lua_show_choice},
			{"show_input", lua_show_input},
			{NULL, NULL}
		};
		luaL_register(L, "Dialog", reg);
		lua_pop(L, 1);
	}
}
