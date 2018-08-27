//=============================================================================
// ■ Dialog for Windows
//=============================================================================

namespace Dialog {
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
	PCWSTR check_context_windows_task_dialog(lua_State* L, int narg, bool question) {
		switch (check_context(L, narg)) {
		case SDL_MESSAGEBOX_INFORMATION:
			return TD_INFORMATION_ICON;
		case SDL_MESSAGEBOX_WARNING:
			return TD_WARNING_ICON;
		case SDL_MESSAGEBOX_ERROR:
			return MB_ERROR_ICON;
		default:
			return NULL;
		}
	}
	//-------------------------------------------------------------------------
	// ● get_hwnd
	//-------------------------------------------------------------------------
	HWND get_hwnd() {
		SDL_SysWMinfo wm_info;
		SDL_VERSION(&wm_info.version);
		SDL_GetWindowWMInfo($window, &wm_info);
		return wm_info.info.win.window;
	}
	//-------------------------------------------------------------------------
	// ● show_message(message[, context])
	//-------------------------------------------------------------------------
	int lua_show_message(lua_State* L) {
		MessageBoxA(
			get_hwnd(),
			Util::utf8_to_os_encoding(luaL_checkstring(L, 1)),
			APPLICATION_TITLE,
			check_context_windows(L, 2, false)
		);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● show_choice(message, buttons[, context])
	//-------------------------------------------------------------------------
	int lua_show_choice(lua_State* L) {
		TASKDIALOGCONFIG config = {0};
		config.cbSize = sizeof(config);
		config.hwndParent = get_hwnd();
		config.hInstance = NULL;
		config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION
			| TDF_USE_COMMAND_LINKS
			| TDF_USE_COMMAND_LINKS_NO_ICON
			| TDF_POSITION_RELATIVE_TO_WINDOW
			| TDF_CAN_BE_MINIMIZED;
		config.dwCommonButtons = TDCBF_CANCEL_BUTTON;
		config.pszWindowTitle = APPLICATION_TITLE;
		config.pszMainIcon = check_context_windows_task_dialog(L, 3, true);
		config.pszMainInstruction = L"Change Password";
		config.pszContent = L"Remember your changed password.";
		config.cButtons = lua_objlen(L, 2);
		TASKDIALOG_BUTTON buttons[config.cButtons];
		for (int i = 0; i < config.cButtons; i++) {
			buttons[i].nButtonID = 100 + i;
			buttons[i].pszButtonText = L"X";
		}
		config.pButtons = buttons;
		config.nDefaultButton = IDCANCEL;
		int buttonid;
		TaskDialogIndirect(&config, &buttonid, NULL, NULL);
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
}
