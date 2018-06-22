//=============================================================================
// ■ MIDIOut
//-----------------------------------------------------------------------------
//   One of the Lua modules for communicating with MIDI devices.
//=============================================================================

namespace MIDIOut {
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
	// ● check_midiout
	//-------------------------------------------------------------------------
	HMIDIOUT check_midiout(lua_State* L, int index) {
		if (lua_istable(L, index)) {
			lua_rawgeti(L, index, 0);
			if (lua_islightuserdata(L, -1)) {
				return (HMIDIOUT) lua_touserdata(L, -1);
			}
		}
		luaL_argerror(L, index, "not a MIDIOut");
		abort();
	}
	//-------------------------------------------------------------------------
	// ● close
	//-------------------------------------------------------------------------
	int lua_close(lua_State* L) {
		switch (midiOutClose(check_midiout(L, lua_upvalueindex(1)))) {
		case MMSYSERR_NOERROR:
			break;
		case MIDIERR_STILLPLAYING:
			luaL_error(L, "Buffers are still in the queue.");
			break;
		case MMSYSERR_INVALHANDLE:
			luaL_error(L, "The specified device handle is invalid.");
			break;
		case MMSYSERR_NOMEM:
			luaL_error(L, "The system is unable to load mapper string description.");
			break;
		default:
			luaL_error(L, "unknown error");
		}
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
		switch (midiOutOpen(&hmo, luaL_checkinteger(L, 1), 0, 0, CALLBACK_NULL)) {
		case MMSYSERR_NOERROR:
			break;
		case MIDIERR_NODEVICE:
			luaL_error(L, "No MIDI port was found.");
			break;
		case MMSYSERR_ALLOCATED:
			luaL_error(L, "The specified resource is already allocated.");
			break;
		case MMSYSERR_BADDEVICEID:
			luaL_error(L, "The specified device identifier is out of range.");
			break;
		case MMSYSERR_INVALPARAM:
			luaL_error(L, "The specified pointer or structure is invalid.");
			break;
		case MMSYSERR_NOMEM:
			luaL_error(L, "The system is unable to allocate or lock memory.");
			break;
		default:
			luaL_error(L, "unknown error");
		}
		lua_pushlightuserdata(L, hmo);
		lua_rawseti(L, -2, 0);
		const luaL_reg reg[] = {
			{"close", lua_close},
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
