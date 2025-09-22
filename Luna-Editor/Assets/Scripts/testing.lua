function sneaky()
	print("sneaky")
end

function Start()
	print("sexy")
end

function Update()
	if(IsKeyDown("a")) then
		Translate(-0.4, 0, 0)
	end
	if(IsKeyDown("d")) then
		Translate(0.4, 0, 0)
	end
end