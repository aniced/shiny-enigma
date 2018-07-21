Item = {styles = require 'c/itemstyles'}

-- update: an update function which will be called regardless of the state
function Item.new(text, help_items, update)
	local self = {}
	self.text = tostring(text)
	self.help_items = help_items
	self.state = nil
	self.update = update

	function self.draw(rect, style)
		if type(style) ~= "table" then
			style = Item.styles[style]
		end
		style.draw_background(rect)
		Typesetting.draw_text({x = rect.x, y = rect.y}, self.text, style.color)
	end

	return self
end
