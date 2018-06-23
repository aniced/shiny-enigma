//=============================================================================
// ■ MIDIIn
//-----------------------------------------------------------------------------
//   One of the Lua modules for communicating with MIDI devices.
//=============================================================================

namespace MIDIIn {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● check_midiin
	//-------------------------------------------------------------------------
	inline HMIDIIN check_midiin(lua_State* L, int index) {
		return (HMIDIIN) Util::check_usertable(L, index, "MIDIIn");
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
	// ● MidiInProc
	//-------------------------------------------------------------------------
	void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{NULL, NULL}
		};
		luaL_register(L, "MIDIIn", reg);
		lua_pop(L, 1);
	}
}
