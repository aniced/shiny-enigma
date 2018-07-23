Item = {styles = Util.dofile 'itemstyles'}

-- text: a string or a function without parameters
-- help_items: an array of items
-- update: an update function which will be called regardless of the state
function Item.new(text, help_items, update)
	local self = {}
	if type(text) ~= "function" then
		text = tostring(text)
	end
	self.text = text
	self.help_items = help_items
	self.state = nil
	self.update = update

	function self.draw(rect, style)
		if type(style) ~= "table" then
			style = Item.styles[style]
		end
		style.draw_background(rect)
		local text = self.text
		if type(text) == "function" then
			text = tostring(text())
		end
		Typesetting.draw_text({x = rect.x, y = rect.y}, text, style.color)
	end

	return self
end
