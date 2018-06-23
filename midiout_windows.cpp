//=============================================================================
// ■ MIDIOut
//-----------------------------------------------------------------------------
//   One of the Lua modules for communicating with MIDI devices.
//=============================================================================

namespace MIDIOut {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● check_midiout
	//-------------------------------------------------------------------------
	inline HMIDIOUT check_midiout(lua_State* L, int index) {
		return (HMIDIOUT) Util::check_usertable(L, index, "MIDIOut");
	}
	//-------------------------------------------------------------------------
	// ● ensure_success
	//-------------------------------------------------------------------------
	void ensure_success(MMRESULT result) {
		static char error_text[MAXERRORLENGTH];
		if (result == MMSYSERR_NOERROR) return;
		switch (midiOutGetErrorText(result, error_text, MAXERRORLENGTH)) {
		case MMSYSERR_NOERROR:
			break;
		case MMSYSERR_BADERRNUM:
		default:
			strcpy(error_text, "unknown");
		}
		luaL_error(L, "midiOut error: %s", Util::os_encoding_to_utf8(error_text));
	}
	//-------------------------------------------------------------------------
	// ● for index, name in devices()
	//-------------------------------------------------------------------------
	int lua_devices_iterator(lua_State* L) {
		int count = lua_tointeger(L, 1);
		int index = lua_tointeger(L, 2);
		if (index < count) {
			MIDIOUTCAPS caps;
			midiOutGetDevCaps(index, &caps, sizeof(caps));
			lua_pushnumber(L, index + 1);
			lua_pushstring(L, caps.szPname);
			return 2;
		} else {
			return 0;
		}
	}
	int lua_devices(lua_State* L) {
		lua_pushcfunction(L, lua_devices_iterator);
		lua_pushnumber(L, midiOutGetNumDevs());
		lua_pushnumber(L, 0);
		return 3;
	}
	//-------------------------------------------------------------------------
	// ● close
	//-------------------------------------------------------------------------
	int lua_close(lua_State* L) {
		ensure_success(midiOutClose(check_midiout(L, lua_upvalueindex(1))));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● send_short_message(byte1, byte2, byte3)
	//-------------------------------------------------------------------------
	int lua_send_short_message(lua_State* L) {
		ensure_success(midiOutShortMsg(
			check_midiout(L, lua_upvalueindex(1)),
			luaL_checkinteger(L, 1)
			| (luaL_checkinteger(L, 2) << 8)
			| (luaL_checkinteger(L, 3) << 16)
		));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● new(index)
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		// t = {}
		lua_createtable(L, 1, 4);
		// t[0] = HMIDIOUT
		HMIDIOUT hmo;
		ensure_success(midiOutOpen(
			&hmo,
			luaL_checkinteger(L, 1),
			0, 0, CALLBACK_NULL
		));
		lua_pushlightuserdata(L, hmo);
		lua_rawseti(L, -2, 0);
		const luaL_reg reg[] = {
			{"close", lua_close},
			{"send_short_message", lua_send_short_message},
			{NULL, NULL}
		};
		Util::instance_register(L, reg);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"new", lua_new_instance},
			{"open", lua_new_instance},
			{"devices", lua_devices},
			{NULL, NULL}
		};
		luaL_register(L, "MIDIOut", reg);
		lua_pop(L, 1);
	}
}
