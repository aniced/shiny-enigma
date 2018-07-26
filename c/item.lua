Item = {styles = Util.dofile 'itemstyles'}

-- text: a string or a function without parameters
-- help: an array of items
-- update: an update function which will be called regardless of the state
function Item.new(text, help, update)
	local self = {}
	if type(text) ~= "function" then
		text = tostring(text)
	end
	self.text = text
	self.help = help
	self.state = nil
	self.update = update

	function self.draw(...)
		Item.draw(self, ...)
	end

	return self
end

function Item.draw(item_or_text, rect, style)
	local text = item_or_text
	if type(item_or_text) == "table" then
		text = item_or_text.text
	end
	if type(text) == "function" then
		text = tostring(text())
	end
	if type(style) ~= "table" then
		style = Item.styles[style]
	end
	style.draw_background(rect)
	Typesetting.draw_text({x = rect.x, y = rect.y}, text, style.color)
end
