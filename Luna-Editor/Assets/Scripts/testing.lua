function Update()
	if(IsKeyDown("a")) then
		SetAcceleration(-5, 0, 0)
	elseif(IsKeyDown("d")) then
		SetAcceleration(5, 0, 0)
	else
		SetAcceleration(0, 0, 0)
	end
end