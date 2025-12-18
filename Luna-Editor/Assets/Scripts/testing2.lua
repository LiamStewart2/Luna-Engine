function Update()
	if(IsKeyDown("a")) then
		SetForce(-5, 0, 0)
	elseif(IsKeyDown("d")) then
		SetForce(5, 0, 0)
	else
		SetForce(0, 0, 0)
	end
end