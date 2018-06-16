Widget_Label = {}

function Widget_Label.new(text)
	local self = Widget_TextBase.new()

	function self.draw()
		Graphics.copy(
			self.rect,
			self.texture,
			self.texture:get_rect()
		)
	end

	return self
end
