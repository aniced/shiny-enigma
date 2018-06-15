BinaryFileWriter = {}

function BinaryFileWriter.new(filename)
	local self = {}
	self.file = io.open(filename, "wb")
	if not self.file then
		error(string.format("error opening binary file %s", filename))
	end

	function self.close()
		self.file:close()
	end

	function self.u8(data)
		self.file:write(string.char(data))
	end

	function self.u16(data)
		self.file:write(string.char(
			bit.band(data, 255),
			bit.band(bit.rshift(data, 8), 255)
		))
	end

	function self.i32(data)
		self.file:write(string.char(
			bit.band(data, 255),
			bit.band(bit.rshift(data, 8), 255),
			bit.band(bit.rshift(data, 16), 255),
			bit.band(bit.rshift(data, 24), 255)
		))
	end

	return self
end
