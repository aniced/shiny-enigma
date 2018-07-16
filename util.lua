-- modname doesn't accept dots as the one require(modname) does.
function Util.subrequire(modname)
	local old_path = package.path
	path, name = modname:match("^(.+)/([^/]-)$")
	if not name then
		path = "."
		name = modname
	end
	package.path = path .. "/?.lua"
	require(name)
	package.path = old_path
end
