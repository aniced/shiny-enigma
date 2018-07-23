Util.path = "."
function Util.dofile(filename)
	local old_path = Util.path
	path, name = filename:match("^(.+)/([^/]-)$")
	if path then
		path = old_path .. "/" .. path
	else
		path = old_path
		name = filename
	end
	if not name:match("%.lua$") then
		name = name .. ".lua"
	end
	Util.path = path
	local ret = {setfenv(assert(loadfile(path .. "/" .. name)), getfenv(2))()}
	Util.path = old_path
	return unpack(ret)
end
