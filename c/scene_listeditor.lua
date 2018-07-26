Scene_ListEditor = {}

function Scene_ListEditor.new(title, data, item_editor_function)
	local self = Scene.new()

	if type(data) ~= "table" then
		error("list data not an array")
	end
	self.data = data
	self.item_editor_function = item_editor_function

	function self.create_item(i)
		return Item.new(function ()
			if i > #self.data then
				return "◆"
			else
				return tostring(self.data[i])
			end
		end)
	end

	function self.sync_items()
		for i = #self.items + 1, #self.data + 1 do
			self.items[i] = create_item(i)
		end
		for i = #self.data + 2, #self.items do
			self.items[i] = nil
		end
	end

	self.items[0] = Item.new(title)
	self.sync_items()
	self.help_items[1] = Item.new("Enter = Insert; Space = Edit; Delete = Remove")

	function self.on_update()
		local i = self.cursor
		if Input.repeated(40) then -- insert
			local item = self.item_editor_function(nil)
			if item then
				table.insert(self.items, i, item)
				self.sync_items()
			end
		end
		if i <= #self.data then
			if Input.repeated(44) then -- edit
				self.items[i] = self.item_editor_function(self.data[i])
			end
			if Input.repeated(76) then -- remove
				table.remove(self.data, i)
				self.sync_items()
			end
		end
	end

	return self
end
