//=============================================================================
// ■ base.cpp
//-----------------------------------------------------------------------------
//   Initialization, application main loop, event handling, etc.
//=============================================================================

void init() {
	if (SDL_Init(0) < 0) error("/* 1st */ SDL_Init() < 0");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		error("/* 2nd */ SDL_Init() < 0");
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (TTF_Init() == -1) error("TTF_Init() == -1");
	$window = SDL_CreateWindow(
		APPLICATION_TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE
	);
	SDL_SetWindowMinimumSize($window, 320, 240);
	$renderer = SDL_CreateRenderer($window, -1, 0);
	SDL_RenderSetIntegerScale($renderer, SDL_TRUE);
	for (int i = 0; i < 2; i++) {
		char filename[16];
		sprintf(filename, "tex%d.png", i);
		$texture[i] = IMG_LoadTexture($renderer, filename);
	}
	for (int i = 0; i < 1; i++) {
		char filename[16];
		sprintf(filename, "fon%d.ttf", i);
		$font[i] = TTF_OpenFontIndex(filename, 12, 0);
		TTF_SetFontHinting($font[i], TTF_HINTING_NONE);
	}
	SDL_ClearError();
	L = luaL_newstate();
	lua_atpanic(L, panic);
	luaL_openlibs(L);
	lua_getglobal(L, "os");
	lua_pushcfunction(L, quit_lua);
	lua_setfield(L, -2, "exit");
	lua_pop(L, 1);
	Graphics::init();
	Window::init();
	if (luaL_dofile(L, "main.lua") != 0) panic(L);
}

void loop() {
	SDL_Event event;
	while (SDL_WaitEventTimeout(&event, 100)) switch (event.type) {
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
}

int main(int argc, char *argv[]) {
	init();
	for (;;) loop();
}
