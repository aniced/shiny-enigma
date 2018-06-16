//=============================================================================
// ■ base.cpp
//-----------------------------------------------------------------------------
//   Initialization, application main loop, event handling, etc.
//=============================================================================

void init(int argc, char* argv[]) {
	if (SDL_Init(0) < 0) error("/* 1st */ SDL_Init() < 0");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		error("/* 2nd */ SDL_Init() < 0");
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (TTF_Init() == -1) error("TTF_Init() == -1");
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
	// load assets
	for (int i = 0; i < 3; i++) {
		char filename[16];
		sprintf(filename, "tex%d.png", i);
		$texture[i] = IMG_LoadTexture($renderer, Util::rtp(filename));
	}
	for (int i = 0; i < 1; i++) {
		char filename[16];
		sprintf(filename, "fon%d.ttf", i);
		$font[i] = TTF_OpenFontIndex(Util::rtp(filename), 20, 0);
		TTF_SetFontHinting($font[i], TTF_HINTING_NONE);
	}
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
	// open game libraries
	UTF8::init();
	Geometry::init();
	Graphics::init();
	Texture::init();
	Window::init();
	Input::init();
	Util::init();
	// _G.on = {}
	lua_newtable(L);
	lua_setglobal(L, "on");
	// execute the main script
	char* script_filename;
	script_filename = Util::rtp("entry.lua");
	if (luaL_dofile(L, script_filename) != 0) panic(L);
	SDL_free(script_filename);
}

void loop() {
	Uint32 frame_start = SDL_GetTicks();
	Input::update();
	Util::call_handler("update");
	Graphics::update();
	Uint32 frame_time = SDL_GetTicks() - frame_start;
	if (frame_time < Graphics::frame_time) {
		SDL_Delay(Graphics::frame_time - frame_time);
	}
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	for (;;) loop();
}
