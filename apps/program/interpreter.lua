Interpreter = {}

function Interpreter.new(program)
	local self = Object.new()
	self.class = Interpreter

	-- Instruction pointer points to the one to be executed.
	self.program = program
	self.ip = {module_id = program.entry_module_id, index = 1}

	-- Jump here when executing Return.
	self.stack = {}
	self.return_value = nil

	function self.run()
		if type(self.program) ~= "table" then
			error("Interpreter: missing program")
		end
	end

	return self
end
