Widget_Textbox = Util.class(Widget_TextBase)

function Widget_Textbox:init()
	Widget_TextBase.init(self)
	self:set_text("")
	self.rect.h = 22
end

function Widget_Textbox:update()
	--[[
	for k = 4, 29 do
		if Input.key_repeated(k) then
			self:set_text(self.text .. string.char(93 + k))
		end
	end
	--]]
	local text = Input.text_get_text()
	if text then
		self:set_text(self.text .. text)
	end
	if Input.key_repeated(42) then
		self:set_text(self.text:sub(1, -2))
	end
end

function Widget_Textbox:draw()
	local patch = {texture = 0, x = 0, y = 16, w = 3, h = 3, t = 1, r = 1, b = 1, l = 1}
	Graphics.draw_9patch(self.rect, patch)
	local cursor_rect
	if self.texture then
		local texture_rect = self.texture:get_rect()
		cursor_rect = {
			x = self.rect.x + 1 + texture_rect.w,
			y = self.rect.y + 2,
			w = 2,
			h = texture_rect.h,
		}
		Graphics.copy(
			{x = self.rect.x + 2, y = self.rect.y + 2},
			self.texture, texture_rect
		)
	else
		cursor_rect = {
			x = self.rect.x + 2,
			y = self.rect.y + 2,
			w = self.rect.w - 4,
			h = self.rect.h - 4,
		}
	end
	Graphics.copy(
		cursor_rect,
		0, {x = 9, y = 3, w = 1, h = 1}
	)
end
