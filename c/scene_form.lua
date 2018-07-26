Scene_Form = {}

--[[ For example,
	fields = {
		{field = nil, display_name = "Subheading 1"},
		{field = "x", value = 0, help_items = {Item.new("𝑥 ∈ ℝ")}},
		{field = "y", value = 0, display_name = "Value"},
		{field = nil}, -- a separator
	}
]]
function Scene_Form.new(title, fields)
	local self = Scene.new()

	self.data = {}
	self.fields = fields

	self.items[0] = Item.new(title)
	function self.create_item(i)
		local v = self.fields[i]
		return Item.new(function ()
			return string.format(
				"%s\ft[6]%s",
				v.display_name or v.field,
				tostring(self.data[v.field])
			)
		end, v.help_items, function ()
			if Input.repeated(40) or Input.repeated(44) then
			end
		end)
	end
	for i, v in ipairs(self.fields) do
		if v.field then
			self.data[v.field] = v.value
			self.items[i] = self.create_item(i)
		else
			self.items[i] = Item.new(v.display_name)
		end
	end
	self.help_items[1] = Item.new("Enter/Space = Edit; Esc = Done")

	function self.on_update()
		if Input.triggered(41) then
			SceneManager.back(self.data)
		end
	end

	return self
end
