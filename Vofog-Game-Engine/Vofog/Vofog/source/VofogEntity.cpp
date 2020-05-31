#include "vpch.h"
#include "VofogEntity.h"



VofogEntity::VofogEntity(){
	
	transform = std::make_shared<VofogTransformComponent>();
	sprite.fileName = "Default Location\0";

}
VofogEntity::~VofogEntity() {
}
int VofogEntity::_LUA_CreateEntity(lua_State* L) {

	VofogEntityManager* entityManager = (VofogEntityManager*)lua_touserdata(L, lua_upvalueindex(1)); assert(entityManager);

	VofogEntity* entityPointer = (VofogEntity * )lua_newuserdata(L, sizeof(VofogEntity));
	new(entityPointer) VofogEntity();
	entityManager->registerEntity((VofogEntity*)entityPointer);
	luaL_getmetatable(L, "EntityMetaTable");
	lua_setmetatable(L, -2);

	lua_newtable(L);
	lua_setuservalue(L, 1);


	lua_getuservalue(L, 1);
	lua_pushstring(L, "transform");
	VofogTransformComponent* transformPointer = (VofogTransformComponent * )lua_newuserdata(L, sizeof(VofogTransformComponent));
	new(transformPointer) VofogTransformComponent(); assert(transformPointer);
	//entityPointer->transform = transformPointer;
	VOFOG_CORE_LOG("UserVal mem addr: {}", (void*)transformPointer);

	luaL_getmetatable(L, "TransformComponentMetaTable");
	lua_setmetatable(L, -2);
	lua_settable(L, -3);


	lua_pushvalue(L, 1);

	return 1;
}

int VofogEntity::_LUA_AttachComponent(lua_State* L) {
	VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -2); assert(luaEntity);
	VofogTransformComponent* luaTrans = (VofogTransformComponent*)lua_touserdata(L, -1); assert(luaTrans);
	//*luaEntity->transform = (*luaTrans);
	return 0;
}
void VofogEntity::moveEntity(float x, float y) {
	//transform->position += glm::vec2(x, y);
}

void VofogEntity::drawEntity() {
	std::string pos = "\n";
	//for (int i = 0; i < transform->position.y; i++) pos += "\n|\n";
	//for (int i = 0; i < transform->position.x; i++) pos += "-\t";
	VOFOG_CORE_LOG("{}$", pos);
}


VofogTransformComponent::VofogTransformComponent() { position = glm::vec2(0); rotation = glm::vec3(0); }
VofogTransformComponent::~VofogTransformComponent() {  }
int VofogTransformComponent::_LUA_CreateTransform(lua_State* L) {

	void* entityPointer = lua_newuserdata(L, sizeof(VofogTransformComponent));
	new(entityPointer) VofogTransformComponent(); assert(entityPointer);
	luaL_getmetatable(L, "TransformComponentMetaTable");
	lua_setmetatable(L, -2);
	return 1;
}

int VofogTransformComponent::_LUA_SetPosition(lua_State* L) {
	VofogTransformComponent* luaEntityTransform = (VofogTransformComponent*)lua_touserdata(L, -3); assert(luaEntityTransform);
	VOFOG_CORE_LOG("SetPos mem addr: {}", (void*)luaEntityTransform);
	float yPos = lua_tonumber(L, -1);
	float xPos = lua_tonumber(L, -2);
	VOFOG_CORE_LOG("Moved from {}, {} to {}, {}", luaEntityTransform->position.x, luaEntityTransform->position.y, xPos, yPos);
	luaEntityTransform->position.x = xPos;
	luaEntityTransform->position.y = yPos;
	return 0;
}

int VofogTransformComponent::_LUA_gc(lua_State* L) {
	VofogTransformComponent* luaEntityTransform = (VofogTransformComponent*)lua_touserdata(L, -1); assert(luaEntityTransform);
	luaEntityTransform->~VofogTransformComponent();
	return 0;
}

int VofogTransformComponent::_LUA_eq(lua_State* L) {
	VofogTransformComponent* luaTransform1 = (VofogTransformComponent*)lua_touserdata(L, -1); assert(luaTransform1);
	VofogTransformComponent* luaTransform2 = (VofogTransformComponent*)lua_touserdata(L, -2); assert(luaTransform2);
	if (luaTransform1->position == luaTransform2->position && luaTransform1->rotation == luaTransform2->rotation) {
		lua_pushboolean(L, true);
	}
	return 1;
}


int VofogTransformComponent::_LUA_index(lua_State* L) {
	assert(lua_isstring(L, -1));
	VofogTransformComponent* luaEntityTransform1 = (VofogTransformComponent*)lua_touserdata(L, -2); assert(luaEntityTransform1);

	const char* index = lua_tostring(L, -1);
	VOFOG_CORE_LOG("Transform: Trying to find a method for \"{}\"", index);
	lua_getglobal(L, "TransformComponent");
	lua_pushstring(L, index);
	lua_rawget(L, -2);
	return 1;
}
int VofogTransformComponent::_LUA_tostring(lua_State* L) {
	VofogTransformComponent* vofogTransformComponent = (VofogTransformComponent*)lua_touserdata(L, -1); assert(vofogTransformComponent);
	glm::vec2 position = vofogTransformComponent->position;
	glm::vec3 rotation = vofogTransformComponent->rotation;

	std::string tostringData = "[LUA] VofogTransformComponent:\n\tPosition: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")\n\t" +
		"Rotation: (" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z) + ")\n";
	lua_pushstring(L, tostringData.c_str());
	return 1;
}