SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture[100] = {NULL};
TTF_Font* font;
lua_State* L;

void quit(int rc) {
	if (TTF_WasInit()) TTF_Quit();
	IMG_Quit();
	if (SDL_WasInit(0)) SDL_Quit();
	exit(rc);
}

void error(const char* m) {
	int rubbish;
	char s[4096];
	sprintf(s, "%s\nSDL_GetError(): %s", m, SDL_GetError());
	SDL_MessageBoxData data;
	SDL_MessageBoxButtonData btn;
	data.flags = SDL_MESSAGEBOX_ERROR;
	data.window = window;
	data.title = APPLICATION_TITLE;
	data.message = s;
	data.numbuttons = 1;
	btn.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
	btn.buttonid = 0;
	btn.text = "Quit";
	data.buttons = &btn;
	data.colorScheme = NULL;
	SDL_ShowMessageBox(&data, &rubbish);
	quit(1);
}

void init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) quit(2);
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (TTF_Init() == -1) error("TTF_Init() == -1");
	window = SDL_CreateWindow(
		APPLICATION_TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE
	);
	SDL_SetWindowMinimumSize(window, 320, 240);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
	for (int i = 0; i < 1; i++) {
		char filename[16];
		sprintf(filename, "tex%d.png", i);
		texture[i] = IMG_LoadTexture(renderer, filename);
	}
	font = TTF_OpenFontIndex("Vera.ttf", 12, 0);
	TTF_SetFontHinting(font, TTF_HINTING_NONE);
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_dofile(L, "main.lua") != 0) error(lua_tostring(L, -1));
}

void loop() {
	SDL_Event event;
	while (SDL_WaitEventTimeout(&event, 1)) switch (event.type) {
		case SDL_QUIT:
			quit(0);
			break;
		case SDL_KEYDOWN:
			lua_getglobal(L, "on_keydown");
			lua_pushnumber(L, event.key.keysym.scancode);
			if (lua_pcall(L, 1, 0, 0) != 0) error(lua_tostring(L, -1));
			break;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture[0], NULL, NULL);
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
	init();
	for (;;) loop();
}
