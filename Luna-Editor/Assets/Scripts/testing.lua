function Update()
	if(IsKeyDown("a")) then
		Translate(-0.4, 0, 0)
	end
	if(IsKeyDown("d")) then
		Translate(0.4, 0, 0)
	end
end