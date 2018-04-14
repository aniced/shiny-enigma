local superclass = Widget_TextBase
local class = Util.class(superclass)

function class:init()
	Widget_TextBase.init(self)
end

function class:update()
end

function class:draw()
	local patch = {texture = 0, x = 0, y = 0, w = 4, h = 4, t = 1, r = 2, b = 2, l = 1}
	if Rect.point_in_rect(Input.mouse(), self.rect) then
		patch.x = patch.x + 5
		patch.y = patch.y + 1
	end
	Graphics.draw_9patch(self.rect, patch)
	Graphics.copy(
		{x = self.rect.x, y = self.rect.y},
		self.texture,
		self.texture:get_rect()
	)
end

return class
