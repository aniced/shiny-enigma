local self = {}
Application = self

self.loaded = {}

function self.load(name)
	if self.loaded[name] then
		return self.loaded[name]
	else
		local function f()
			Util.dofile(string.format("apps/%s/main.lua", name))
		end
		local env = setmetatable({}, {__index = _G})
		setfenv(f, env)()
		self.loaded[name] = env
		return env
	end
end
