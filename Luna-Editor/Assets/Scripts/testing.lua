function Update()
	if(IsKeyDown("a")) then
		Translate(-1, 0, 0)
	end
	if(IsKeyDown("d")) then
		Translate(1, 0, 0)
	end
end