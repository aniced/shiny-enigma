local class = Util.class()

function class:init(filename)
	self.file = io.open(filename, "wb")
	if not self.file then
		error(string.format("error opening binary file %s", filename))
	end
end

function class:close()
	self.file:close()
end

function class:u8(data)
	self.file:write(string.char(data))
end

function class:u16(data)
	self.file:write(string.char(
		bit.band(data, 255),
		bit.band(bit.rshift(data, 8), 255)
	))
end

function class:i32(data)
	self.file:write(string.char(
		bit.band(data, 255),
		bit.band(bit.rshift(data, 8), 255),
		bit.band(bit.rshift(data, 16), 255),
		bit.band(bit.rshift(data, 24), 255)
	))
end

return class
