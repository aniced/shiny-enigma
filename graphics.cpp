//=============================================================================
// ■ Graphics
//-----------------------------------------------------------------------------
//   A Lua module for drawing with the following classes.
//   Texture — a userdata of a pointer to SDL_Texture
//   Font    — a userdata of a pointer to TTF_Font
//=============================================================================

namespace Graphics {
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		SDL_SetRenderDrawColor($renderer, 0, 0, 0, 255);
		SDL_RenderClear($renderer);
		Util::call_handler("paint");
		SDL_RenderPresent($renderer);
	}
	//-------------------------------------------------------------------------
	// ● check_texture
	//-------------------------------------------------------------------------
	SDL_Texture* check_texture(lua_State* L, int index) {
		if (lua_isuserdata(L, index)) {
			return *((SDL_Texture**) luaL_checkudata(L, index, "Texture"));
		} else {
			return $texture[luaL_checkinteger(L, index)];
		}
	}
	//-------------------------------------------------------------------------
	// ● check_font
	//-------------------- ----------------------------------------
	TTF_Font* check_font(lua_State* L, int index) {
		return *((TTF_Font**) luaL_checkudata(L, index, "Font"));
	}
	//-------------------------------------------------------------------------
	// ● get_fps & set_fps
	//-------------------------------------------------------------------------
	int fps = 60;
	Uint32 frame_time = 1000 / fps;
	int get_fps(lua_State* L) {
		lua_pushnumber(L, fps);
		return 1;
	}
	int set_fps(lua_State* L) {
		fps = luaL_checkinteger(L, 1);
		frame_time = 1000 / fps;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_size()
	//   Use this size for drawing.
	//-------------------------------------------------------------------------
	int get_size(lua_State* L) {
		SDL_Rect rect = {0, 0, -1, -1};
		SDL_GetRendererOutputSize($renderer, &rect.w, &rect.h);
		lua_pushnumber(L, rect.w);
		lua_pushnumber(L, rect.h);
		return 2;
	}
	//-------------------------------------------------------------------------
	// ● get_rect()
	//-------------------------------------------------------------------------
	int get_rect(lua_State* L) {
		SDL_Rect rect = {0, 0, -1, -1};
		SDL_GetRendererOutputSize($renderer, &rect.w, &rect.h);
		Rect::create_rect(L, &rect);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● copy(dest_rect, texture, src_rect)
	//   texture: texture ID or reference
	//-------------------------------------------------------------------------
	int copy(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 2);
		SDL_Rect src, dest;
		Rect::to_rect(L, 1, &dest);
		Rect::to_rect(L, 3, &src);
		if (!dest.w) dest.w = src.w;
		if (!dest.h) dest.h = src.h;
		if (SDL_RenderCopy($renderer, texture, &src, &dest) < 0) {
			return luaL_error(L, "SDL_RenderCopy() < 0");
		}
	}
	//-------------------------------------------------------------------------
	// ● draw_line(point1, point2, ...)
	//-------------------------------------------------------------------------
	int draw_line(lua_State* L) {
		int count = lua_gettop(L);
		SDL_Point points[count];
		for (int i = 0; i < count; i++) {
			Rect::to_point(L, i + 1, &points[i]);
		}
		SDL_RenderDrawLines($renderer, points, count);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_color(color)
	//-------------------------------------------------------------------------
	int set_color(lua_State* L) {
		SDL_Color color;
		Util::to_color(L, 1, &color);
		SDL_SetRenderDrawColor($renderer, color.r, color.g, color.b, color.a);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_blend(mode)
	//   mode: "copy", "blend", "add", "mod"
	//-------------------------------------------------------------------------
	int set_blend(lua_State* L) {
		const char* s = lua_tostring(L, 1);
		SDL_BlendMode mode;
		if (strcmp(s, "copy") == 0) {
			mode = SDL_BLENDMODE_NONE;
		} else if (strcmp(s, "blend") == 0 || !*s) {
			mode = SDL_BLENDMODE_BLEND;
		} else if (strcmp(s, "add") == 0) {
			mode = SDL_BLENDMODE_ADD;
		} else if (strcmp(s, "mod") == 0) {
			mode = SDL_BLENDMODE_MOD;
		}
		SDL_SetRenderDrawBlendMode($renderer, mode);
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
	// ● fill_rect(rect)
	//-------------------------------------------------------------------------
	int fill_rect(lua_State* L) {
		SDL_Rect rect;
		Rect::to_rect(L, 1, &rect);
		SDL_RenderFillRect($renderer, &rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● draw_9patch(dest_rect, patch_info)
	//   patch_info has the following keys.
	//   - texture: texture ID or reference
	//   - x, y, w and h: source rectangle
	//   - t, r, b and l: margins
	//-------------------------------------------------------------------------
	int draw_9patch(lua_State* L) {
		// rectangles for SDL_RenderCopy
		SDL_Rect src_rect, dest_rect;
		// get the texture
		lua_getfield(L, 2, "texture");
		SDL_Texture* texture = check_texture(L, -1);
		lua_pop(L, 1);
		// retrieve the coordinates, sizes and margins
		Rect::to_rect(L, 1, &dest_rect);
		int x = dest_rect.x, y = dest_rect.y, w = dest_rect.w, h = dest_rect.h;
		Rect::to_rect(L, 2, &src_rect);
		int sx = src_rect.x, sy = src_rect.y, sw = src_rect.w, sh = src_rect.h;
		lua_getfield(L, 2, "t"); int t = lua_tointeger(L, -1);
		lua_getfield(L, 2, "r"); int r = lua_tointeger(L, -1);
		lua_getfield(L, 2, "b"); int b = lua_tointeger(L, -1);
		lua_getfield(L, 2, "l"); int l = lua_tointeger(L, -1);
		lua_getfield(L, 2, "fill"); bool fill = lua_toboolean(L, -1);
		if (lua_isnil(L, -1)) fill = true;
		lua_pop(L, 5);
		#define RCP(sx, sy, sw, sh, dx, dy, dw, dh) \
			src_rect.x = sx; src_rect.y = sy; src_rect.w = sw; src_rect.h = sh; \
			dest_rect.x = dx; dest_rect.y = dy; dest_rect.w = dw; dest_rect.h = dh; \
			SDL_RenderCopy($renderer, texture, &src_rect, &dest_rect)
		RCP(sx, sy, l, t, x, y, l, t);
		RCP(sx + l, sy, sw - l - r, t, x + l, y, w - l - r, t);
		RCP(sx + sw - r, sy, r, t, x + w - r, y, r, t);
		RCP(sx, sy + t, l, sh - t - b, x, y + t, l, h - t - b);
		RCP(sx + sw - r, sy + t, r, sh - t - b, x + w - r, y + t, r, h - t - b);
		RCP(sx, sy + sh - b, l, b, x, y + h - b, l, b);
		RCP(sx + l, sy + sh - b, sw - l - r, b, x + l, y + h - b, w - l - r, b);
		RCP(sx + sw - r, sy + sh - b, r, b, x + w - r, y + h - b, r, b);
		if (fill) {
			RCP(sx + l, sy + t, sw - l - r, sh - t - b, x + l, y + t, w - l - r, h - t - b);
		}
		#undef RCP
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● render_text(font, text) → texture reference
	//   font: font ID or reference
	//-------------------------------------------------------------------------
	int render_text(lua_State* L) {
		TTF_Font* font;
		if (lua_isuserdata(L, 1)) {
			font = check_font(L, 1);
		} else {
			font = $font[luaL_checkinteger(L, 1)];
		}
		const char* text = luaL_checkstring(L, 2);
		SDL_Color color = {255, 255, 255, 255};
		SDL_Surface* surface = TTF_RenderUTF8_Blended($font[0], text, color);
		if (!surface) error("TTF_RenderUTF8() == NULL");
		SDL_Texture** texture = (SDL_Texture**) lua_newuserdata(L, sizeof(void*));
		luaL_getmetatable(L, "Texture");
		lua_setmetatable(L, -2);
		*texture = SDL_CreateTextureFromSurface($renderer, surface);
		SDL_FreeSurface(surface);
		return 1;
	}
	//-------------------------------------------------------------------------
	// ● ~Font()
	//-------------------------------------------------------------------------
	int font_gc(lua_State* L) {
		TTF_Font* font = check_font(L, 1);
		TTF_CloseFont(font);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● ~Texture()
	//-------------------------------------------------------------------------
	int texture_gc(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
		SDL_DestroyTexture(texture);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● Texture:get_rect() → rect
	//-------------------------------------------------------------------------
	int texture_get_rect(lua_State* L) {
		SDL_Texture* texture = check_texture(L, 1);
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
		SDL_Texture* texture = check_texture(L, 1);
		SDL_Color color;
		Util::to_color(L, 2, &color);
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		{
			const luaL_reg reg[] = {
				{"get_fps", get_fps},
				{"set_fps", set_fps},
				{"get_size", get_size},
				{"get_rect", get_rect},
				{"copy", copy},
				{"render_text", render_text},
				{"set_color", set_color},
				{"set_blend", set_blend},
				{"draw_rect", draw_rect},
				{"fill_rect", fill_rect},
				{"draw_line", draw_line},
				{"draw_9patch", draw_9patch},
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
		{
			const luaL_reg reg[] = {
				{"__gc", font_gc},
				{NULL, NULL}
			};
			luaL_newmetatable(L, "Font");
			luaL_register(L, NULL, reg);
			lua_setfield(L, -1, "__index");
		}
	}
}
