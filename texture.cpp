//=============================================================================
// ■ Texture
//-----------------------------------------------------------------------------
//   A Lua class for handling textures.
//=============================================================================

namespace Texture {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	const char* tname = "Graphics.Texture";
	//-------------------------------------------------------------------------
	// ● check_texture
	//-------------------------------------------------------------------------
	SDL_Texture* check_texture(lua_State* L, int index) {
		if (lua_isuserdata(L, index)) {
			return *((SDL_Texture**) luaL_checkudata(L, index, tname));
		} else {
			return $texture[luaL_checkinteger(L, index)];
		}
	}
	//-------------------------------------------------------------------------
	// ● create_texture
	//-------------------------------------------------------------------------
	void create_texture(lua_State* L, SDL_Texture* texture) {
		SDL_Texture** ud = (SDL_Texture**) lua_newuserdata(L, sizeof(void*));
		luaL_getmetatable(L, tname);
		lua_setmetatable(L, -2);
		*ud = texture;
	}
	//-------------------------------------------------------------------------
	// ● new(filename)
	//-------------------------------------------------------------------------
	int lua_new_instance(lua_State* L) {
		const char* filename = lua_tostring(L, 1);
		create_texture(L, IMG_LoadTexture($renderer, Util::rtp(filename)));
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● render_text(font, text) → texture reference
	//   font: font ID or reference
	//-------------------------------------------------------------------------
	int lua_render_text(lua_State* L) {
		TTF_Font* font;
		if (lua_isuserdata(L, 1)) {
			font = Graphics::check_font(L, 1);
		} else {
			font = $font[luaL_checkinteger(L, 1)];
		}
		const char* text = luaL_checkstring(L, 2);
		SDL_Color color = {255, 255, 255, 255};
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (!surface) luaL_error(L, "TTF_RenderUTF8() == NULL");
		create_texture(L, SDL_CreateTextureFromSurface($renderer, surface));
		SDL_FreeSurface(surface);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● ~Texture()
	//-------------------------------------------------------------------------
	int lua_gc(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		SDL_DestroyTexture(texture);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_rect(self) → rect
	//-------------------------------------------------------------------------
	int lua_get_rect(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		Uint32 format;
		int w, h, access;
		SDL_QueryTexture(texture, &format, &access, &w, &h);
		SDL_Rect rect = {0, 0, w, h};
		Geometry::create_rect(L, &rect);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● set_color(self, color)
	//-------------------------------------------------------------------------
	int lua_set_color(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		SDL_Color color;
		Util::to_color(L, 2, &color);
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_blend(self, mode)
	//-------------------------------------------------------------------------
	int lua_set_blend(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		SDL_SetTextureBlendMode(texture, Graphics::check_blend(L, 2));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		const luaL_reg reg[] = {
			{"__gc", lua_gc},
			{"render_text", lua_render_text},
			{"get_rect", lua_get_rect},
			{"set_color", lua_set_color},
			{"set_blend", lua_set_blend},
			{NULL, NULL}
		};
		luaL_newmetatable(L, tname);
		luaL_register(L, NULL, reg);
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_setglobal(L, "Texture");
	}
}
