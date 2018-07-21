Scene_Hello = {}

function Scene_Hello.new()
	local self = Scene.new()
	self.column_count = 3

	self.items[0] = Item.new("你好！")
	self.items[1] = Item.new("Program", nil, function ()
		if Input.triggered(40) or Input.triggered(44) then
			local app = Application.load("program")
			SceneManager.call(app.Scene_Program)
		end
	end)
	for i = 2, 29 do
		self.items[i] = Item.new(string.format(
			"%d→%f\fc[r255g0b0]{\fi[16]}",
			i, math.sqrt(i))
		)
	end
	self.help_items[1] = Item.new("F1 = Fire a message; Esc = Exit")

	function self.on_update()
		if Input.triggered(58) then
			self.items[0] = Item.new(tostring(SceneManager.call(Scene_Message.new())))
		end
		if Input.triggered(41) then
			os.exit()
		end
	end

	return self
end
