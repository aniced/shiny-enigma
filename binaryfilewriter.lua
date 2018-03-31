BinaryFileWriter = Util.class()

function BinaryFileWriter:init(filename)
	self.file = io.open(filename, "wb")
	if not self.file then
		error(string.format("error opening binary file %s", filename))
	end
end

function BinaryFileWriter:close()
	self.file:close()
end

function BinaryFileWriter:u8(data)
	self.file:write(string.char(data))
end

function BinaryFileWriter:u16(data)
	self.file:write(string.char(
		bit.band(data, 255),
		bit.band(bit.rshift(data, 8), 255)
	))
end

function BinaryFileWriter:i32(data)
	self.file:write(string.char(
		bit.band(data, 255),
		bit.band(bit.rshift(data, 8), 255),
		bit.band(bit.rshift(data, 16), 255),
		bit.band(bit.rshift(data, 24), 255)
	))
end
