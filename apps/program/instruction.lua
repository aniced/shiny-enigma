Instruction = {}

function Instruction.new()
	local self = {}
	self.class = Instruction

	function self.write_c(stream)
		error("method not implemented")
	end

	return self
end
