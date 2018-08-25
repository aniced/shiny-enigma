dofile 'util.lua'
Util.dofile 'application'
Util.dofile 'typesetting'

print(JSON.stringify({
	a = {"\0\a\b\f\n\r\t\v", true, false, "\" \\"},
	b = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
	c = 12345e300,
}))
function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg >= 1 then
		Util.dofile 'c/main.lua'
	else
		Util.dofile 'g/main.lua'
	end
end
