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
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"devices", lua_devices},
			{NULL, NULL}
		};
		luaL_register(L, "MIDIOut", reg);
		lua_pop(L, 1);
	}
}
