function Update()
	rotationSpeed = 50;
	if(IsKeyDown("e")) then
		AddForce(-movementSpeed, 0, 0)
	end
	if(IsKeyDown("q")) then
		AddForce(movementSpeed, 0, 0)
	end
end