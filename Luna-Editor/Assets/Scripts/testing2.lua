function Update()
	if(IsKeyDown("w")) then
		Translate(0, 0.3, 0)
	end
	if(IsKeyDown("s")) then
		Translate(0, -0.3, 0)
	end
end