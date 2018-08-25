//=============================================================================
// ■ base.cpp
//-----------------------------------------------------------------------------
//   Initialization, application main loop, event handling, etc.
//=============================================================================

void init(int argc, char* argv[]) {
	if (SDL_Init(0) < 0) sdlerror("/* 1st */ SDL_Init() < 0");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (SDL_InitSubSystem(
		SDL_INIT_VIDEO
		| SDL_INIT_TIMER
		| SDL_INIT_AUDIO
	) < 0) {
		sdlerror("/* 2nd */ SDL_Init() < 0");
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (TTF_Init() == -1) sdlerror("TTF_Init() == -1");
	Util::init_rtp();
	$window = SDL_CreateWindow(
		APPLICATION_TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE
	);
	SDL_SetWindowMinimumSize($window, 320, 240);
	$renderer = SDL_CreateRenderer($window, -1, 0);
	SDL_RenderSetIntegerScale($renderer, SDL_TRUE);
	SDL_ClearError();
	// initialize Lua
	L = luaL_newstate();
	lua_atpanic(L, panic);
	luaL_openlibs(L);
	// set _G.arg
	lua_createtable(L, argc, 0);
	for (int i = 0; i < argc; i++) {
		lua_pushstring(L, argv[i]);
		lua_rawseti(L, -2, i);
	}
	lua_setglobal(L, "arg");
	// replace package.path
	lua_getglobal(L, "package");
	lua_pushstring(L, Util::rtp("?.lua"));
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);
	// inject os.exit
	lua_getglobal(L, "os");
	lua_pushcfunction(L, quit_lua);
	lua_setfield(L, -2, "exit");
	lua_pop(L, 1);
	// remove io.open, io.popen, io.tmpfile and io.close
	lua_getglobal(L, "io");
	lua_pushnil(L); lua_setfield(L, -2, "open");
	lua_pushnil(L); lua_setfield(L, -2, "popen");
	lua_pushnil(L); lua_setfield(L, -2, "tmpfile");
	lua_pushnil(L); lua_setfield(L, -2, "close");
	lua_pop(L, 1);
	// remove package, require and module
	lua_pushnil(L); lua_setglobal(L, "package");
	lua_pushnil(L); lua_setglobal(L, "require");
	lua_pushnil(L); lua_setglobal(L, "module");
	// open game libraries
	Object::init();
	UTF8::init();
	IOStream::init();
	Geometry::init();
	Graphics::init();
	Texture::init();
	Window::init();
	Input::init();
	Audio::init();
	#ifdef ENABLE_MIDI
	MIDIIn::init();
	MIDIOut::init();
	#endif
	Font::init();
	JSON::init();
	Util::init();
	// _G.on = {}
	lua_newtable(L);
	lua_setglobal(L, "on");
	// execute the main script
	{
		lua_pushcfunction(L, errfunc);
		int errfunc_index = lua_gettop(L);
		char* script_filename;
		script_filename = Util::rtp("entry.lua");
		if (luaL_loadfile(L, script_filename) || lua_pcall(L, 0, 0, errfunc_index)) {
			panic(L);
		}
		lua_pop(L, 1);
	}
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	for (;;) {
		Input::update();
		Util::call_handler("update");
		Graphics::update();
	}
}
