namespace Graphics {
	//-------------------------------------------------------------------------
	// ● copy(dest_rect, texture, src_rect)
	//   texture: texture ID or reference
	//-------------------------------------------------------------------------
	int copy(lua_State* L) {
		SDL_Texture* texture;
		if (lua_isuserdata(L, 2)) {
			texture = *((SDL_Texture**) luaL_checkudata(L, 2, "Texture"));
		} else {
			texture = $texture[luaL_checkinteger(L, 2)];
		}
		SDL_Rect src, dest;
		Rect::to_rect(L, 1, &dest);
		Rect::to_rect(L, 3, &src);
		if (!dest.w) dest.w = src.w;
		if (!dest.h) dest.h = src.h;
		if (SDL_RenderCopy($renderer, texture, &src, &dest) < 0) {
			error("SDL_RenderCopy() < 0");
		}
	}
	//-------------------------------------------------------------------------
	// ● draw_rect(rect)
	//-------------------------------------------------------------------------
	int draw_rect(lua_State* L) {
		SDL_Rect rect;
		Rect::to_rect(L, 1, &rect);
		SDL_RenderDrawRect($renderer, &rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● render_text(text) → texture reference
	//-------------------------------------------------------------------------
	int render_text(lua_State* L) {
		SDL_Color color = {255, 255, 255, 255};
		const char* text = luaL_checkstring(L, 1);
		SDL_Surface* surface = TTF_RenderUTF8_Blended($font, text, color);
		if (!surface) error("TTF_RenderUTF8() == NULL");
		SDL_Texture** texture = (SDL_Texture**) lua_newuserdata(L, sizeof(void*));
		luaL_getmetatable(L, "Texture");
		lua_setmetatable(L, -2);
		*texture = SDL_CreateTextureFromSurface($renderer, surface);
		SDL_FreeSurface(surface);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● to_texture
	//-------------------------------------------------------------------------
	SDL_Texture* to_texture(lua_State* L, int index) {
		return *((SDL_Texture**) luaL_checkudata(L, index, "Texture"));
	}
	//-------------------------------------------------------------------------
	// ● ~Texture()
	//-------------------------------------------------------------------------
	int texture_gc(lua_State* L) {
		SDL_Texture* texture = to_texture(L, 1);
		SDL_DestroyTexture(texture);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● Texture:get_rect() → rect
	//-------------------------------------------------------------------------
	int texture_get_rect(lua_State* L) {
		SDL_Texture* texture = to_texture(L, 1);
		Uint32 format;
		int w, h, access;
		SDL_QueryTexture(texture, &format, &access, &w, &h);
		SDL_Rect rect = {0, 0, w, h};
		Rect::create_rect(L, &rect);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● Texture:set_color(color)
	//-------------------------------------------------------------------------
	int texture_set_color(lua_State* L) {
		SDL_Texture* texture = to_texture(L, 1);
		SDL_Color color;
		Util::to_color(L, 2, &color);
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		return 0;
	}
	void init() {
		{
			const luaL_reg reg[] = {
				{"copy", copy},
				{"render_text", render_text},
				{"draw_rect", draw_rect},
				{NULL, NULL}
			};
			luaL_register(L, "Graphics", reg);
			lua_pop(L, 1);
		}
		{
			const luaL_reg reg[] = {
				{"__gc", texture_gc},
				{"get_rect", texture_get_rect},
				{"set_color", texture_set_color},
				{NULL, NULL}
			};
			luaL_newmetatable(L, "Texture");
			luaL_register(L, NULL, reg);
			lua_setfield(L, -1, "__index");
		}
	}
}
