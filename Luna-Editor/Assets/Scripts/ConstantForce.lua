function Update()
	movementSpeed = 5;
	SetForce(movementSpeed, 0, 0);

	if(GetPositionX() > 10) then
		SetForce(0, 0, 0);
		SetAcceleration(0, 0, 0);
		SetVelocity(0, 0, 0);
		SetPosition(-10, -0.5, 7);
	end
end