Scene_Hello = {}

function Scene_Hello.new()
	local self = Scene.new()
	self.column_count = 3

	self.title = "你好！"
	self.items[1] = Item.new("Program", nil, function ()
		if Input.triggered(40) or Input.triggered(44) then
			local app = Application.load("program")
			SceneManager.call(app.Scene_Program.new())
		end
	end)
	self.items[2] = Item.new("Form", nil, function ()
		if Input.triggered(40) or Input.triggered(44) then
			SceneManager.call(Scene_Form.new("Hello Forms!", {
				{field = nil, display_name = "Subheading 1"},
				{field = "x", value = 0, help = {"𝑥 ∈ ℝ"}},
				{field = nil},
				{field = "y", value = 0, display_name = "Value"},
			}))
		end
	end)
	for i = 3, 29 do
		self.items[i] = Item.new(string.format(
			"%d→%f\fc[r255g0b0]{\fi[16]}",
			i, math.sqrt(i))
		)
	end
	self.help[1] = "F1 = Fire a message; Esc = Exit"

	function self.on_update()
		if Input.triggered(58) then
			self.title = tostring(SceneManager.call(Scene_Message.new()))
		end
		if Input.triggered(41) then
			os.exit()
		end
	end

	return self
end
