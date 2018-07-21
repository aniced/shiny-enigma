Scene_Hello = {}

function Scene_Hello.new()
	local self = Scene.new()
	self.cursor = 26
	self.column_count = 3

	self.items[0] = Item.new("你好！")
	for i = 1, 30 do
		self.items[i] = Item.new(string.format(
			"%d→%f\fc[r255g0b0]{\fi[16]}",
			i, math.sqrt(i))
		)
	end
	self.help_items[1] = Item.new("Space = Fire a message; Esc = Exit")

	function self.on_update()
		if Input.triggered(44) then
			self.items[1] = Item.new(SceneManager.call(Scene_Message.new()))
		end
		if Input.triggered(41) then
			os.exit()
		end
	end

	return self
end
