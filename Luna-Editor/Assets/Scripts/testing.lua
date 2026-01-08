function Update()
	if(IsKeyDown("a")) then
		AddForce(-5, 0, 0)
	elseif(IsKeyDown("d")) then
		AddForce(5, 0, 0)
	end
end