dofile 'util.lua'
Util.dofile 'application'
Util.dofile 'typesetting'

Dialog.show_choice("啥玩意",{"去","我去"})
function on.update()
	-- only for first frame as this function itself
	-- will be replaced the moment it is called
	if #arg >= 1 then
		Util.dofile 'c/main.lua'
	else
		Util.dofile 'g/main.lua'
	end
end
