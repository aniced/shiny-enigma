Scene_Message = {}

function Scene_Message.new()
	local self = Scene.new()
	local i, j = 0, 0
	self.items[0] = Item.new("Message")
	self.items[1] = Item.new("Press Ｚ on me", {
		Item.new("Z = increment; Space = return"),
	}, function ()
		if Input.repeated(29) then
			i = i + 1
		end
		if Input.pressed(29) then
			j = j + 1
			self.items[2].text = string.format("%d\ft[4]%d", i, j)
		end
		if Input.triggered(44) then
			SceneManager.back(1.05)
		end
	end)
	self.items[2] = Item.new("---")
	self.help_items[1] = Item.new("")
	return self
end
