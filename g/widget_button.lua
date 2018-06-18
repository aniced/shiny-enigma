Widget_Button = {}

function Widget_Button.new()
	local self = Widget_TextBase.new()

	function self.update()
	end

	function self.draw()
		local patch = {texture = Texture.stock[0], x = 8, y = 0, w = 4, h = 4, t = 1, r = 2, b = 2, l = 1}
		if Geometry.point_in_rect(Input.mouse(), self.rect) then
			patch.x = patch.x + 5
			patch.y = patch.y + 1
		end
		Graphics.draw_9patch(self.rect, patch)
		Graphics.copy(
			Geometry.shrink_rect(self.rect, self.padding),
			self.texture,
			self.texture.get_rect()
		)
	end

	return self
end
