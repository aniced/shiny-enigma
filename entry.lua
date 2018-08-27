dofile 'util.lua'
Util.dofile 'application'
Util.dofile 'typesetting'

print(JSON.stringify({
	a = {"\0\a\b\f\n\r\t\v", true, false, "\" \\"},
	b = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
	c = 12345e300,
}))
local p = JSON.parse([[
{
	"b" : [ null , [ null , 1 , 0 , 0 ] , [ null , 0 , 1 , 0 ] , [ null , 0 , 0 , 1 ] ] ,
	"a" : [ null , "\u0000\u0007\b\f\n\r\t\u000b" , true , false , "\" \\" ] ,
	"c" : 1.2345e+304
}
]])
print(string.format("%q", p.a[1]), p.a[2], p.a[3], p.a[4])
print(p.b[0], p.b[1][1], p.b[1][2], p.b[2][1], p.b[2][2])
function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg >= 1 then
		Util.dofile 'c/main.lua'
	else
		Util.dofile 'g/main.lua'
	end
end
