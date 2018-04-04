require 'binaryfilereader'
require 'binaryfilewriter'

function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg > 3 then
		dofile 'conmode.lua'
	else
		dofile 'main.lua'
	end
end
