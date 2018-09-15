//=============================================================================
// ■ Audio
//-----------------------------------------------------------------------------
//   A Lua module for playing back.
//=============================================================================

namespace Audio {
	//-------------------------------------------------------------------------
	// ● check_audio
	//-------------------------------------------------------------------------
	inline SDL_AudioDeviceID check_audio(lua_State* L, int index) {
		#if __x86_64__
		long long id = (uintptr_t) Util::check_usertable(L, index, "audio device");
		return (SDL_AudioDeviceID) id;
		#else
		return (SDL_AudioDeviceID) Util::check_usertable(L, index, "audio device");
		#endif
	}
	//-------------------------------------------------------------------------
	// ● for index, name in devices()
	//-------------------------------------------------------------------------
	int lua_devices_iterator(lua_State* L) {
		int count = lua_tointeger(L, 1);
		int index = lua_tointeger(L, 2);
		if (index < count) {
			lua_pushnumber(L, index + 1);
			lua_pushstring(L, SDL_GetAudioDeviceName(index, false));
			return 2;
		} else {
			return 0;
		}
	}
	int lua_devices(lua_State* L) {
		lua_pushcfunction(L, lua_devices_iterator);
		lua_pushnumber(L, SDL_GetNumAudioDevices(false));
		lua_pushnumber(L, 0);
		return 3;
	}
	//-------------------------------------------------------------------------
	// ● close
	//-------------------------------------------------------------------------
	int lua_close(lua_State* L) {
		SDL_CloseAudioDevice(check_audio(L, lua_upvalueindex(1)));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● new(name)
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		// t = {}
		lua_createtable(L, 1, 4);
		// t[0] = (void*) AudioDeviceID
		SDL_AudioSpec desired, obtained;
		SDL_AudioDeviceID id = SDL_OpenAudioDevice(
			luaL_checkstring(L, 1),
			false,
			&desired,
			&obtained,
			0
		);
		if (!id) Util::sdlerror(L, "SDL_OpenAudioDevice == 0");
		lua_pushlightuserdata(L, (void*) id);
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
		luaL_register(L, "Audio", reg);
		lua_pop(L, 1);
	}
}
