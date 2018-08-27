//=============================================================================
// ■ Dialog
//-----------------------------------------------------------------------------
//   A Lua module providing access to native or emulated modal dialogs.
//   A few optional parameters are common among the preset dialogs:
//       context: "error", "warning", "information" (the default) or ""
//           "info" shows a question mark in dialogs that require the user
//           to input, or an “i” icon in dialogs that do not.
//           However, Windows doesn't allow the use of the question mark icon
//           anymore, only for the new icon looks like the help icon.
//=============================================================================

namespace Dialog {
	//-------------------------------------------------------------------------
	// ● check_context
	//   The name should be ‘opt_context.’
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
	// ● init
	//-------------------------------------------------------------------------
	int lua_show_message(lua_State* L);
	int lua_show_choice(lua_State* L);
	int lua_show_input(lua_State* L);
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
