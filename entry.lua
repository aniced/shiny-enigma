dofile 'util.lua'
Util.dofile 'application'
Util.dofile 'typesetting'

function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg >= 1 then
		Util.dofile 'c/main.lua'
	else
		Util.dofile 'g/main.lua'
	end
end
