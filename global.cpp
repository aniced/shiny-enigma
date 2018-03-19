//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   Global variables and global helper functions are stored here.
//=============================================================================
#define APPLICATION_TITLE ""

SDL_Window* $window;
SDL_Renderer* $renderer;
SDL_Texture* $texture[100] = {NULL};
TTF_Font* $font;
lua_State* L;

#define module_begin(name, n) \
	void init_script_interface() { \
		const char* const module_name = #name; \
		lua_createtable(L, 0, n); \
		int index = lua_gettop(L);
#define module_function(name) \
		lua_pushcfunction(L, name); \
		lua_setfield(L, index, #name);
#define module_end \
		lua_setglobal(L, module_name); \
	}

void quit(int rc) {
	puts("Quitting");
	if (TTF_WasInit()) TTF_Quit();
	IMG_Quit();
	if (SDL_WasInit(0)) SDL_Quit();
	exit(rc);
}

int quit_lua(lua_State* L) {
	quit(luaL_optint(L, 1, EXIT_SUCCESS));
	return 0;
}

void msgbox(const char* s) {
	SDL_MessageBoxData data;
	data.flags = SDL_MESSAGEBOX_ERROR;
	data.window = $window;
	data.title = APPLICATION_TITLE;
	data.message = s;
	data.numbuttons = 1;
	SDL_MessageBoxButtonData btn;
	btn.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
	btn.buttonid = 0;
	btn.text = "Quit";
	data.buttons = &btn;
	data.colorScheme = NULL;
	SDL_ShowMessageBox(&data, &data.numbuttons);
}

void error(const char* m) {
	char s[4096];
	sprintf(s, "%s\nSDL_GetError(): %s", m, SDL_GetError());
	msgbox(s);
	quit(1);
}

int panic(lua_State* L) {
	msgbox(lua_tostring(L, -1));
	quit(1); // never returning
}
