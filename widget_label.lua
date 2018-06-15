Widget_Label = {}

function Widget_Label.new(text)
	local self = Widget_TextBase.new()

	function self.draw()
		Graphics.copy(
			{x = self.rect.x, y = self.rect.y},
			self.texture,
			self.texture:get_rect()
		)
	end

	return self
end
