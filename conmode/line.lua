Line = {styles = require 'conmode/linestyles'}

function Line.get_rect(i)
	return {x = 0, y = i * WLH, w = Graphics.w, h = WLH}
end

-- update: an update function which will be called regardless of the state
function Line.new(text, help_items, update)
	local self = {}
	self.text = tostring(text)
	self.help_items = help_items
	self.state = nil
	self.update = update

	function self.draw(i, style)
		if type(style) ~= "table" then
			style = Line.styles[style]
		end
		local rect = Line.get_rect(i)
		style.draw_background(i)
		Typesetting.draw_text({
			x = 0,
			y = i * WLH,
		}, self.text, style.color)
	end

	return self
end
