Variable = {}

function Variable.new()
	local self = Object.new()
	self.class = Variable

	self.constant = false

	return self
end
