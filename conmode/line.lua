local class = Util.class()
class.styles = require 'conmode/linestyles'

function class.get_rect(i)
	return {x = 0, y = i * WLH, w = Graphics.get_rect().w, h = WLH}
end

-- update: an update function which will be called regardless of the state
function class:init(text, help_items, update)
	self.text = tostring(text)
	self.help_items = help_items
	self.state = nil
	self.update = update
end

function class:draw(i, style)
	if type(style) ~= "table" then
		style = class.styles[style]
	end
	local rect = class.get_rect(i)
	style.draw_background(i)
	Typesetting.draw_text({
		x = 0,
		y = i * WLH,
	}, self.text, style.color)
end

return class
