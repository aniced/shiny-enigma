//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   Global variables and global helper functions are stored here.
//=============================================================================

using namespace std;
#define APPLICATION_TITLE ""
#define EXPAND(x) x
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

SDL_Window* $window;
SDL_Renderer* $renderer;
SDL_Texture* $texture[100] = {NULL};
TTF_Font* $font[10] = {NULL};
lua_State* L;
char msgbox_buf[4096];

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
	msgbox(m);
	quit(1);
}

void sdlerror(const char* m) {
	sprintf(msgbox_buf, "%s\nSDL_GetError(): %s", m, SDL_GetError());
	error(msgbox_buf);
}

int panic(lua_State* L) {
	strcpy(msgbox_buf, lua_tostring(L, -1));
	fprintf(stderr, "%s\n", msgbox_buf);
	msgbox(msgbox_buf);
	quit(1); // never returning
	abort(); // never getting here
}

int errfunc(lua_State* L) {
	// return debug.traceback(message:gsub(": ", ":\n"))
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_pushstring(L, luaL_gsub(L, lua_tostring(L, -1), ": ", ":\n"));
	lua_call(L, 1, 1);
	return 1;
}
