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
	Uint32 last_frame_end_tick;
	// Normally all drawing operations apply to this buffer, so that
	// while logic and drawing seem mixed together in the client code,
	// they are actually separated, so that effects like transitions
	// can be done without requesting the client to redraw themselves.
	SDL_Texture* framebuffer;
	struct transition_state {
		bool active = false;
		int duration;
		int frame;
		SDL_Texture* old_texture;
		// ‘new_texture’ should be the framebuffer.
	} transition_state;
	bool frame_persistent = false;
	double frame_opacity = 1.0;
	// These brightnesses are multiplied.
	double brightness = 1.0;
	double brightness_for_fade = 1.0;
	struct fade_state {
		bool active = false;
		int duration;
		int frame;
		bool go_dark; // true = fade out
	} fade_state;
	//-------------------------------------------------------------------------
	// ● get_fps & set_fps
	//-------------------------------------------------------------------------
	int fps = 60;
	Uint32 frame_ticks = 1000 / fps;
	int lua_get_fps(lua_State* L) {
		lua_pushnumber(L, fps);
		return 1;
	}
	int lua_set_fps(lua_State* L) {
		fps = luaL_checkinteger(L, 1);
		frame_ticks = 1000 / fps;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● update_transition
	//-------------------------------------------------------------------------
	void update_transition() {
		// draw the frozen picture first
		SDL_RenderCopy($renderer, transition_state.old_texture, NULL, NULL);
		if (transition_state.duration) {
			// duration != 0: transitioning
			int alpha = 255 * transition_state.frame / transition_state.duration;
			SDL_SetTextureAlphaMod(framebuffer, alpha);
			SDL_RenderCopy($renderer, framebuffer, NULL, NULL);
			transition_state.frame++;
			if (transition_state.frame > transition_state.duration) {
				transition_state.active = false;
				SDL_DestroyTexture(transition_state.old_texture);
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
			// interpolate the brightness for fade
			brightness_for_fade = (double) fade_state.frame / fade_state.duration;
			if (fade_state.go_dark) brightness_for_fade = 1 - brightness_for_fade;
			fade_state.frame++;
			if (fade_state.frame > fade_state.duration) {
				fade_state.active = false;
			}
		}
		// multiply the brightnesses
		Uint8 b = brightness * brightness_for_fade * 255;
		SDL_SetRenderDrawBlendMode($renderer, SDL_BLENDMODE_MOD);
		SDL_SetRenderDrawColor($renderer, b, b, b, 255);
		SDL_RenderFillRect($renderer, NULL);
	}
	//-------------------------------------------------------------------------
	// ● update
	//-------------------------------------------------------------------------
	void update() {
		if (transition_state.active && transition_state.frame == 0) {
			// skip the call to paint to avoid an early update
		} else {
			SDL_SetRenderTarget($renderer, framebuffer);
			Util::call_handler("paint");
			SDL_SetRenderTarget($renderer, NULL);
		}
		if (!frame_persistent) {
			SDL_SetRenderDrawColor($renderer, 0, 0, 0, 255);
			SDL_RenderClear($renderer);
		}
		if (transition_state.active) {
			update_transition();
		} else {
			Uint8 a = frame_opacity * 255;
			SDL_SetTextureAlphaMod(framebuffer, a);
			SDL_RenderCopy($renderer, framebuffer, NULL, NULL);
		}
		update_brightness();
		SDL_RenderPresent($renderer);
		// adjust the speed to fps
		Uint32 this_frame_end_tick = SDL_GetTicks();
		if (this_frame_end_tick - last_frame_end_tick < frame_ticks) {
			SDL_Delay(frame_ticks - (this_frame_end_tick - last_frame_end_tick));
			last_frame_end_tick = this_frame_end_tick;
		}
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
	// ● copy(dest_point, texture, src_rect)
	//   texture: texture ID or reference
	//-------------------------------------------------------------------------
	int lua_copy(lua_State* L) {
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
	int lua_stretch_copy(lua_State* L) {
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
	//-------------------------------------------------------------------------
	int lua_tile(lua_State* L) {
		SDL_Texture* texture = Texture::check_texture(L, 2);
		SDL_Rect src, dest, piece_dest;
		Geometry::check_rect(L, 1, &dest);
		Geometry::check_rect(L, 3, &src);
		piece_dest.w = src.w;
		piece_dest.h = src.h;
		int piece_w = dest.w / src.w;
		int piece_h = dest.h / src.h;
		bool clip = (dest.w % src.w) || (dest.h % src.h);
		if (clip) {
			SDL_RenderSetClipRect($renderer, &dest);
			piece_w++;
			piece_h++;
		}
		for (int piece_y = 0; piece_y < piece_h; piece_y++) {
			piece_dest.y = dest.y + piece_y * src.h;
			for (int piece_x = 0; piece_x < piece_w; piece_x++) {
				piece_dest.x = dest.x + piece_x * src.w;
				if (SDL_RenderCopy($renderer, texture, &src, &piece_dest) < 0) {
					Util::sdlerror(L, "SDL_RenderCopy() < 0");
					abort();
				}
			}
		}
		if (clip) SDL_RenderSetClipRect($renderer, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● draw_line(point1, point2, ...)
	//-------------------------------------------------------------------------
	int lua_draw_line(lua_State* L) {
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
	int lua_set_color(lua_State* L) {
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
	int lua_set_blend(lua_State* L) {
		SDL_SetRenderDrawBlendMode($renderer, check_blend(L, 1));
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● draw_rect(rect)
	//-------------------------------------------------------------------------
	int lua_draw_rect(lua_State* L) {
		SDL_Rect rect;
		Geometry::to_rect(L, 1, &rect);
		SDL_RenderDrawRect($renderer, &rect);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● fill_rect(rect)
	//-------------------------------------------------------------------------
	int lua_fill_rect(lua_State* L) {
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
	int lua_draw_9patch(lua_State* L) {
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
		if (!texture) {
			Util::sdlerror(L, "SDL_CreateTexture(SDL_TEXTUREACCESS_TARGET) == NULL");
		}
		return texture;
	}
	//-------------------------------------------------------------------------
	// ● get_animation_enabled()
	//   set_animation_enabled(enabled)
	//-------------------------------------------------------------------------
	bool animation_enabled = true;
	int lua_get_animation_enabled(lua_State* L) {
		lua_pushboolean(L, animation_enabled);
		return 1;
	}
	int lua_set_animation_enabled(lua_State* L) {
		animation_enabled = Util::check_boolean(L, 1);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● freeze()
	//-------------------------------------------------------------------------
	int lua_freeze(lua_State* L) {
		if (!animation_enabled) return 0;
		// fill in the transition state partially
		transition_state.active = false;
		transition_state.duration = 0;
		// capture the current picture
		transition_state.old_texture = create_target_texture();
		SDL_SetRenderTarget($renderer, transition_state.old_texture);
		Util::call_handler("paint");
		SDL_SetRenderTarget($renderer, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● transition(duration)
	//-------------------------------------------------------------------------
	int lua_transition(lua_State* L) {
		if (!animation_enabled) return 0;
		// fill in the transition state
		transition_state.duration = luaL_checkint(L, 1);
		if (transition_state.duration <= 0) {
			return luaL_error(L, "duration must be positive");
		}
		transition_state.frame = 0;
		transition_state.active = true;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● stop_transition()
	//   This function makes no difference if any of the following:
	//   - animation is disabled
	//   - freeze() is called but not transition()
	//-------------------------------------------------------------------------
	int lua_stop_transition(lua_State* L) {
		transition_state.active = false;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_frame_persistent()
	//   set_frame_persistent(persistent)
	//-------------------------------------------------------------------------
	int lua_get_frame_persistent(lua_State* L) {
		lua_pushboolean(L, frame_persistent);
		return 1;
	}
	int lua_set_frame_persistent(lua_State* L) {
		frame_persistent = Util::check_boolean(L, 1);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_frame_opacity()
	//   set_frame_opacity(opacity ∈ [0, 1])
	//-------------------------------------------------------------------------
	int lua_get_frame_opacity(lua_State* L) {
		lua_pushnumber(L, frame_opacity);
		return 1;
	}
	int lua_set_frame_opacity(lua_State* L) {
		frame_opacity = Util::clamp(luaL_checknumber(L, 1), 0.0, 1.0);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● get_brightness()
	//   set_brightness(brightness ∈ [0, 1])
	//-------------------------------------------------------------------------
	int lua_get_brightness(lua_State* L) {
		lua_pushnumber(L, brightness);
		return 1;
	}
	int lua_set_brightness(lua_State* L) {
		brightness = Util::clamp(luaL_checknumber(L, 1), 0.0, 1.0);
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● fade_in/fade_out(duration = 10)
	//-------------------------------------------------------------------------
	int lua_fade(lua_State* L) {
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
	//-------------------------------------------------------------------------
	int lua_stop_fade(lua_State* L) {
		fade_state.active = false;
		brightness_for_fade = 1.0;
		return 0;
	}
	//-------------------------------------------------------------------------
	// ● init
	//-------------------------------------------------------------------------
	void init() {
		framebuffer = create_target_texture();
		SDL_SetTextureBlendMode(framebuffer, SDL_BLENDMODE_BLEND);
		const luaL_reg reg[] = {
			{"x", NULL},
			{"y", NULL},
			{"w", NULL},
			{"h", NULL},
			{"get_fps", lua_get_fps},
			{"set_fps", lua_set_fps},
			{"copy", lua_copy},
			{"stretch_copy", lua_stretch_copy},
			{"tile", lua_tile},
			{"set_color", lua_set_color},
			{"set_blend", lua_set_blend},
			{"draw_rect", lua_draw_rect},
			{"fill_rect", lua_fill_rect},
			{"draw_line", lua_draw_line},
			{"draw_9patch", lua_draw_9patch},
			{"get_animation_enabled", lua_get_animation_enabled},
			{"set_animation_enabled", lua_set_animation_enabled},
			{"freeze", lua_freeze},
			{"transition", lua_transition},
			{"stop_transition", lua_stop_transition},
			{"get_frame_persistent", lua_get_frame_persistent},
			{"set_frame_persistent", lua_set_frame_persistent},
			{"get_frame_opacity", lua_get_frame_opacity},
			{"set_frame_opacity", lua_set_frame_opacity},
			{"get_brightness", lua_get_brightness},
			{"set_brightness", lua_set_brightness},
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
		lua_pushcclosure(L, lua_fade, 1);
		lua_setfield(L, -2, "fade_in");
		lua_pushboolean(L, true);
		lua_pushcclosure(L, lua_fade, 1);
		lua_setfield(L, -2, "fade_out");
		// (registry)[update] = Graphics
		lua_pushlightuserdata(L, (void*) update);
		lua_pushvalue(L, -2);
		lua_settable(L, LUA_REGISTRYINDEX);
		lua_pop(L, 1);
	}
}
