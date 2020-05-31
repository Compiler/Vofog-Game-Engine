
function Vec2(x, y) end

local vec2_meta = {
	__add = function(a, b)
		return Vec2(a.x + b.x, a.y + b.y)
		
	end,

	__tostring = function(self)
		return "Vector2 : (".. self.x ..", " .. self.y .. ")"
		
	end,
}

function Vec2(x, y)
	local v = {
		x = x or 0,
		y = y or 0
	}
	setmetatable(v, vec2_meta)
	return v
end

local a = Vec2(10, 10)
local b = Vec2(20, 20)
local c = Vec2(0,0)
c = a+b

--print("c.x = " .. c.x)
--print(c)


-------------------------



trans = TransformComponent.create()
trans:setPosition(1, 2);
print(trans)


entity = Entity.create()
print(entity)

entity:attachComponent(trans)
print(entity)
entity.transform:setPosition(4,4)
print(entity)
