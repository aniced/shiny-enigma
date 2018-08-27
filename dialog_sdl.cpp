//=============================================================================
// ■ Dialog for generic platforms
//=============================================================================

namespace Dialog {
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
}
