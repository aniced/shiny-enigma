Scene_Message = {}

function Scene_Message.new()
	local self = Scene.new()
	local i, j = 0, 0
	self.column_count = 2

	self.items[0] = Item.new("Message")
	self.items[1] = Item.new("Press Ｚ on me", {
		Item.new("Z = increment; Esc = return"),
	}, function ()
		if Input.repeated(29) then
			i = i + 1
		end
		if Input.pressed(29) then
			j = j + 1
			self.items[2].text = string.format("%d～%d", i, j)
		end
		if Input.triggered(41) then
			SceneManager.back(os.clock())
		end
	end)
	self.items[2] = Item.new("---")
	self.help_items[1] = Item.new("")
	return self
end
