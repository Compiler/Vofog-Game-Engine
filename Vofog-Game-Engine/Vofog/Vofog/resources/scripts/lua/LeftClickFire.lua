
function init()

end

function update()
	leftClick = Input.isMouseButtonDown(0)
	rightClick = Input.isMouseButtonDown(1)
	clickCount = 0
	player = ecs:getEntityByName("Player")
	impulseVal = 1
	if(Input.isKeyDown(KeyHome)) then vprint("KeyHome is pressed") end
	if(Input.isKeyDown(KeyF4)) then vprint("KeyF4 is pressed") end
	if(Input.isKeyDown(KeyScrollLock)) then vprint("KeyScrollLock is pressed") end
	if(Input.isKeyDown(KeyKeyPadDivide)) then vprint("KeyKeyPadDivide is pressed") end

	if(Input.isKeyDown(KeyA)) then vprint("A is pressed") end
	if(Input.isKeyDown(KeyW)) then vprint("W pressed") end
	if(Input.isKeyDown(KeyS)) then vprint("S is pressed") end
	if(Input.isKeyDown(KeyD)) then vprint("D is pressed") end
	if(leftClick)then
		vprint("Left mouse button click! " .. clickCount)	
		for var1=0,10,1 do 
			ecs:addRenderableEntity(0, -5 + var1, -3)
		end
	end

	if(rightClick)then
		vprint("Right mouse button click!")	
	end

	if(Input.isKeyDown(KeyUp)) then
		ecs:getPhysics(player).body:setState(4)
		ecs:getPhysics(player).body:applyCentralImpulse(btvec3.new(0,impulseVal * 10,0))
	end
	if(Input.isKeyDown(KeyRight)) then
		ecs:getPhysics(player).body:setState(1)
		ecs:getPhysics(player).body:applyCentralImpulse(btvec3.new(impulseVal,0,0))
	end
	if(Input.isKeyDown(KeyLeft)) then
		ecs:getPhysics(player).body:setState(1)
		ecs:getPhysics(player).body:applyCentralImpulse(btvec3.new(-impulseVal,0,0))
	end
	
end