Util.subpath = "."

function Util.subrequire(modname)
	local old_path = package.path
	local old_subpath = Util.subpath
	-- normalize modname
	modname = modname:gsub("[%.\\]", "/")
	if modname:match("^/") then
		error("invalid modname")
	end
	path, name = modname:match("^(.+)/([^/]-)$")
	if not path then
		path = old_subpath
		name = modname
	else
		path = old_subpath .. "/" .. path
	end
	package.path = path .. "/?.lua"
	Util.subpath = path
	require(name)
	package.path = old_path
	Util.subpath = old_subpath
end
