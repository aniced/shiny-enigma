require 'util'
require 'typesetting'
Util.subrequire 'apps/program/main'

function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg >= 1 then
		dofile 'c/main.lua'
	else
		dofile 'g/main.lua'
	end
end
