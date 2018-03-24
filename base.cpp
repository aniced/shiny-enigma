//=============================================================================
// ■ base.cpp
//-----------------------------------------------------------------------------
//   Initialization, application main loop, event handling, etc.
//=============================================================================

const char* $base_path = NULL;

char* rtp(const char* path) {
	static char s[256];
	strcpy(s, $base_path);
	strcat(s, path);
	return s;
}

void init() {
	if (SDL_Init(0) < 0) error("/* 1st */ SDL_Init() < 0");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		error("/* 2nd */ SDL_Init() < 0");
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (TTF_Init() == -1) error("TTF_Init() == -1");
	$base_path = SDL_GetBasePath();
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
	for (int i = 0; i < 2; i++) {
		char filename[16];
		sprintf(filename, "tex%d.png", i);
		$texture[i] = IMG_LoadTexture($renderer, rtp(filename));
	}
	for (int i = 0; i < 1; i++) {
		char filename[16];
		sprintf(filename, "fon%d.ttf", i);
		$font[i] = TTF_OpenFontIndex(rtp(filename), 12, 0);
		TTF_SetFontHinting($font[i], TTF_HINTING_NONE);
	}
	SDL_ClearError();
	// initialize Lua
	L = luaL_newstate();
	lua_atpanic(L, panic);
	luaL_openlibs(L);
	// inject os.exit
	lua_getglobal(L, "os");
	lua_pushcfunction(L, quit_lua);
	lua_setfield(L, -2, "exit");
	lua_pop(L, 1);
	// open game libraries
	Graphics::init();
	Window::init();
	Util::init();
	// execute the main script
	char* script_filename;
	script_filename = rtp("main.lua");
	script_filename = SDL_iconv_utf8_locale(script_filename);
	if (luaL_dofile(L, script_filename) != 0) panic(L);
	SDL_free(script_filename);
}

void loop() {
	SDL_Event event;
	Uint32 frame_start = SDL_GetTicks();
	while (SDL_PollEvent(&event)) switch (event.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_KEYDOWN:
			lua_getglobal(L, "on_keydown");
			if (lua_isnil(L, -1)) {
				lua_pop(L, 1);
				break;
			} else {
				lua_pushnumber(L, event.key.keysym.scancode);
				lua_call(L, 1, 0);
			}
			break;
	}
	SDL_SetRenderDrawColor($renderer, 0, 0, 0, 255);
	SDL_RenderClear($renderer);
	lua_getglobal(L, "on_paint");
	lua_call(L, 0, 0);
	SDL_RenderPresent($renderer);
	Uint32 frame_time = SDL_GetTicks() - frame_start;
	if (frame_time < Graphics::frame_time) {
		SDL_Delay(Graphics::frame_time - frame_time);
	}
}

int main(int argc, char *argv[]) {
	init();
	for (;;) loop();
}
