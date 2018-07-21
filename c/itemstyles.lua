return {
	normal = {
		color = {r = 255, g = 255, b = 255},
		draw_background = function (rect)
			Graphics.tile(rect, Texture.stock[0], {x = 32, y = 0, w = 8, h = 8})
		end,
	},
	null = {
		color = {r = 0, g = 0, b = 0},
		draw_background = function (rect)
			Graphics.tile(rect, Texture.stock[0], {x = 40, y = 0, w = 8, h = 8})
		end,
	},
	selected = {
		color = {r = 45, g = 54, b = 9},
		draw_background = function (rect)
			local patch = {
				texture = Texture.stock[0],
				x = 17, y = 0, w = 5, h = 5,
				t = 2, r = 2, b = 2, l = 2
			}
			Graphics.draw_9patch(rect, patch)
		end,
	},
	help = {
		color = {r = 147, g = 236, b = 147},
		draw_background = function (rect)
			Graphics.stretch_copy(rect, Texture.stock[0], {x = 0, y = 6, w = 1, h = 1})
		end,
	},
	title = {
		color = {r = 147, g = 236, b = 147},
		draw_background = function (rect)
			Graphics.stretch_copy(rect, Texture.stock[0], {x = 0, y = 6, w = 1, h = 1})
		end,
	},
}
