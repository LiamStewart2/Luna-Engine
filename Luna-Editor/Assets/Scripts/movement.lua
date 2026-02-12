function Update()
	movementSpeed = 50;
	if(IsKeyDown("a")) then
		AddForce(-movementSpeed, 0, 0)
	end
	if(IsKeyDown("d")) then
		AddForce(movementSpeed, 0, 0)
	end
	if(IsKeyDown("w")) then
		AddForce(0, 0, -movementSpeed);
	end
	if(IsKeyDown("s")) then
		AddForce(0, 0, movementSpeed);
    end
end