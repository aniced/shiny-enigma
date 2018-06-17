//=============================================================================
// ■ Graphics
//-----------------------------------------------------------------------------
//   A Lua module for drawing with the following classes.
//   Texture — a userdata of a pointer to SDL_Texture
//   Font    — a userdata of a pointer to TTF_Font
//=============================================================================

namespace Texture {
	SDL_Texture* check_texture(lua_State* L, int index);
}

namespace Graphics {
	//-------------------------------------------------------------------------
	// ● Module variables
	//-------------------------------------------------------------------------
	struct transition_state {
		bool active = false;
		int duration;
		int frame;
		SDL_Texture* old_texture;
		SDL_Texture* new_texture;
	} transition_state;
	double brightness = 1.0;
	struct fade_state {
		bool active = false;
		int duration;
		int frame;
		bool go_dark; // true = fade out
	} fade_state;
	//-------------------------------------------------------------------------
	// ● update_transition
	//-------------------------------------------------------------------------
	void update_transition() {
		// draw the frozen picture first
		SDL_RenderCopy($renderer, transition_state.old_texture, NULL, NULL);
		if (transition_state.duration) {
			// duration != 0: transitioning
			int alpha = 255 * transition_state.frame / transition_state.duration;
			SDL_SetTextureAlphaMod(transition_state.new_texture, alpha);
			SDL_RenderCopy($renderer, transition_state.new_texture, NULL, NULL);
			transition_state.frame++;
			if (transition_state.frame > transition_state.duration) {
				transition_state.active = false;
				SDL_DestroyTexture(transition_state.old_texture);
				SDL_DestroyTexture(transition_state.new_texture);
			}
		} else {
			// duration == 0: freezing
		}
	}
	//-------------------------------------------------------------------------
	// ● update_brightness
	//-------------------------------------------------------------------------
	void update_brightness() {
		if (fade_state.active) {
			// interpolate the brightness
			brightness = (double) fade_state.frame / fade_state.duration;
			if (fade_state.go_dark) brightness = 1 - brightness;
			fade_state.frame++;
			if (fade_state.frame > fade_state.duration) {
				fade_state.active = false;
			}
		}
		Uint8 b = brightness * 255;
		SDL_SetTextureBlendMode($texture[1], SDL_BLENDMODE_MOD);
		SDL_Rect src = {240 + (b & 15), 240 + (b >> 4), 1, 1};
		SDL_RenderCopy($renderer, $texture[1], &src, NULL);
	}
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		SDL_SetRenderDrawColor($renderer, 0, 0, 0, 255);
		SDL_RenderClear($renderer);
		if (transition_state.active) {
			update_transition();
		} else {
			Util::call_handler("paint");
		}
		update_brightness();
		SDL_RenderPresent($renderer);
	}
	//-------------------------------------------------------------------------
	// ● check_font
	//-------------------- ----------------------------------------
	TTF_Font* check_font(lua_State* L, int index) {
		return *((TTF_Font**) luaL_checkudata(L, index, "Font"));
	}
	//-------------------------------------------------------------------------
	// ● set_size
	//   Call this function when the window size is changed to update
	//   the field w and h in this module.
	//-------------------------------------------------------------------------
	void set_size(int w, int h) {
		lua_pushlightuserdata(L, (void*) update);
		lua_gettable(L, LUA_REGISTRYINDEX);
		lua_pushnumber(L, w); lua_setfield(L, -2, "w");
		lua_pushnumber(L, h); lua_setfield(L, -2, "h");
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
	// ● copy(dest_point, texture, src_rect)
	//   texture: texture ID or reference
	//-------------------------------------------------------------------------
	int copy(lua_State* L) {
		SDL_Texture* texture = Texture::check_texture(L, 2);
		SDL_Rect src, dest;
		Geometry::to_rect(L, 1, &dest);
		Geometry::check_rect(L, 3, &src);
		dest.w = src.w;
		dest.h = src.h;
		if (SDL_RenderCopy($renderer, texture, &src, &dest) < 0) {
			return luaL_error(L, "SDL_RenderCopy() < 0");
		}
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● stretch_copy(dest_rect, texture, src_rect)
	//-------------------------------------------------------------------------
	int stretch_copy(lua_State* L) {
		SDL_Texture* texture = Texture::check_texture(L, 2);
		SDL_Rect src, dest;
		Geometry::check_rect(L, 1, &dest);
		Geometry::check_rect(L, 3, &src);
		if (SDL_RenderCopy($renderer, texture, &src, &dest) < 0) {
			return luaL_error(L, "SDL_RenderCopy() < 0");
		}
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● tile(dest_rect, texture, src_rect)
	//   The texture will be tiled using a loop. From this point of view,
	//   it may be slower than stretching with the copy function.
	//   The destination rectangle will not be fully covered if it can't be
	//   divided exactly into source rectangles.
	//-------------------------------------------------------------------------
	int tile(lua_State* L) {
		SDL_Texture* texture = Texture::check_texture(L, 2);
		SDL_Rect src, dest, dest1;
		Geometry::check_rect(L, 1, &dest);
		Geometry::check_rect(L, 3, &src);
		dest1.w = src.w;
		dest1.h = src.h;
		for (int ty = 0; ty < dest.h / src.h; ty++) {
			dest1.y = dest.y + ty * src.h;
			for (int tx = 0; tx < dest.w / src.w; tx++) {
				dest1.x = dest.x + tx * src.w;
				if (SDL_RenderCopy($renderer, texture, &src, &dest1) < 0) {
					return luaL_error(L, "SDL_RenderCopy() < 0");
				}
			}
		}
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● draw_line(point1, point2, ...)
	//-------------------------------------------------------------------------
	int draw_line(lua_State* L) {
		int count = lua_gettop(L);
		SDL_Point points[count];
		for (int i = 0; i < count; i++) {
			Geometry::to_point(L, i + 1, &points[i]);
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
	// ● check_blend
	//   "copy", "blend", "add", "mod"
	//-------------------------------------------------------------------------
	SDL_BlendMode check_blend(lua_State* L, int index) {
		const char* s = luaL_checkstring(L, index);
		if (strcmp(s, "copy") == 0) {
			return SDL_BLENDMODE_NONE;
		} else if (strcmp(s, "blend") == 0) {
			return SDL_BLENDMODE_BLEND;
		} else if (strcmp(s, "add") == 0) {
			return SDL_BLENDMODE_ADD;
		} else if (strcmp(s, "mod") == 0) {
			return SDL_BLENDMODE_MOD;
		} else {
			luaL_argerror(L, index, "unknown blend mode");
			abort(); // never getting here
		}
	}
	//-------------------------------------------------------------------------
	// ● set_blend(mode)
	//-------------------------------------------------------------------------
	int set_blend(lua_State* L) {
		SDL_SetRenderDrawBlendMode($renderer, check_blend(L, 1));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● draw_rect(rect)
	//-------------------------------------------------------------------------
	int draw_rect(lua_State* L) {
		SDL_Rect rect;
		Geometry::to_rect(L, 1, &rect);
		SDL_RenderDrawRect($renderer, &rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● fill_rect(rect)
	//-------------------------------------------------------------------------
	int fill_rect(lua_State* L) {
		SDL_Rect rect;
		Geometry::to_rect(L, 1, &rect);
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
		SDL_Texture* texture = Texture::check_texture(L, -1);
		lua_pop(L, 1);
		// retrieve the coordinates, sizes and margins
		Geometry::to_rect(L, 1, &dest_rect);
		int x = dest_rect.x, y = dest_rect.y, w = dest_rect.w, h = dest_rect.h;
		Geometry::to_rect(L, 2, &src_rect);
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
	// ● ~Font()
	//-------------------------------------------------------------------------
	int font_gc(lua_State* L) {
		TTF_Font* font = check_font(L, 1);
		TTF_CloseFont(font);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● create_target_texture
	//-------------------------------------------------------------------------
	SDL_Texture* create_target_texture() {
		int w, h;
		SDL_GetRendererOutputSize($renderer, &w, &h);
		SDL_Texture* texture = SDL_CreateTexture(
			$renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_TARGET,
			w, h
		);
		if (!texture) error("SDL_CreateTexture(SDL_TEXTUREACCESS_TARGET) == NULL");
		return texture;
	}
	//-------------------------------------------------------------------------
	// ● get_animation_enabled()
	//   set_animation_enabled(enabled)
	//-------------------------------------------------------------------------
	bool animation_enabled = true;
	int get_animation_enabled(lua_State* L) {
		lua_pushboolean(L, animation_enabled);
		return 1;
	}
	int set_animation_enabled(lua_State* L) {
		animation_enabled = Util::check_boolean(L, 1);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● freeze()
	//-------------------------------------------------------------------------
	int freeze(lua_State* L) {
		if (!animation_enabled) return 0;
		// fill in the transition state partially
		transition_state.active = false;
		transition_state.duration = 0;
		// capture the current picture
		transition_state.old_texture = create_target_texture();
		SDL_SetRenderTarget($renderer, transition_state.old_texture);
		update();
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● transition(duration = 10)
	//-------------------------------------------------------------------------
	int transition(lua_State* L) {
		if (!animation_enabled) return 0;
		// fill in the transition state
		transition_state.duration = luaL_optint(L, 1, 10);
		if (!transition_state.duration) return luaL_error(L, "duration cannot be zero");
		transition_state.frame = 0;
		transition_state.new_texture = create_target_texture();
		SDL_SetTextureBlendMode(transition_state.new_texture, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget($renderer, transition_state.new_texture);
		update(); // paint
		SDL_SetRenderTarget($renderer, NULL);
		transition_state.active = true;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● stop_transition()
	//   This function makes no difference if any of the following:
	//   - animation is disabled
	//   - freeze() is called but not transition()
	//-------------------------------------------------------------------------
	int stop_transition(lua_State* L) {
		transition_state.active = false;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● set_brightness(brightness ∈ [0, 1])
	//-------------------------------------------------------------------------
	int set_brightness(lua_State* L) {
		brightness = Util::clamp(luaL_checknumber(L, 1), 0.0, 1.0);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● fade_in/fade_out(duration = 10)
	//-------------------------------------------------------------------------
	int fade(lua_State* L) {
		if (!animation_enabled) return 0;
		// fill in the fade state
		fade_state.duration = luaL_optint(L, 1, 10);
		if (!fade_state.duration) return luaL_error(L, "duration cannot be zero");
		fade_state.frame = 0;
		fade_state.go_dark = lua_toboolean(L, lua_upvalueindex(1));
		fade_state.active = true;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● stop_fade
	//   The brightness will remain!
	//-------------------------------------------------------------------------
	int stop_fade(lua_State* L) {
		fade_state.active = false;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		{
			const luaL_reg reg[] = {
				{"x", NULL},
				{"y", NULL},
				{"w", NULL},
				{"h", NULL},
				{"get_fps", get_fps},
				{"set_fps", set_fps},
				{"copy", copy},
				{"stretch_copy", stretch_copy},
				{"tile", tile},
				{"set_color", set_color},
				{"set_blend", set_blend},
				{"draw_rect", draw_rect},
				{"fill_rect", fill_rect},
				{"draw_line", draw_line},
				{"draw_9patch", draw_9patch},
				{"get_animation_enabled", get_animation_enabled},
				{"set_animation_enabled", set_animation_enabled},
				{"freeze", freeze},
				{"transition", transition},
				{"stop_transition", stop_transition},
				{"set_brightness", set_brightness},
				{"fade_in", NULL},
				{"fade_out", NULL},
				{NULL, NULL}
			};
			luaL_register(L, "Graphics", reg);
			// Graphics.x = Graphics.y = 0
			lua_pushnumber(L, 0); lua_setfield(L, -2, "x");
			lua_pushnumber(L, 0); lua_setfield(L, -2, "y");
			// Graphics.fade_in = fade with false
			// Graphics.fade_out = fade with true
			lua_pushboolean(L, false);
			lua_pushcclosure(L, fade, 1);
			lua_setfield(L, -2, "fade_in");
			lua_pushboolean(L, true);
			lua_pushcclosure(L, fade, 1);
			lua_setfield(L, -2, "fade_out");
			// (registry)[update] = Graphics
			lua_pushlightuserdata(L, (void*) update);
			lua_pushvalue(L, -2);
			lua_settable(L, LUA_REGISTRYINDEX);
			lua_pop(L, 1);
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
