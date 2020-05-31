#pragma once
#include <Vofog.h>
#include <VofogEntityManager.h>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


}
struct VOFOG_API VofogSpriteComponent {
	const char* fileName;
};
struct VOFOG_API VofogTransformComponent {
	glm::vec2 position;
	glm::vec3 rotation;
	VofogTransformComponent();
	VofogTransformComponent& operator=(const VofogTransformComponent& other) {
		position = other.position;
		rotation = other.rotation;
		return *this;
	}

	const char* toString() {
		static std::string data;
		data = std::string("(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")\n").c_str();
		return data.c_str();
	}
	void setPosition(float x, float y) { position.x = x; position.y = y; };
	~VofogTransformComponent();

	static int _LUA_CreateTransform(lua_State* L);
	static int _LUA_SetPosition(lua_State* L);
	static int _LUA_gc(lua_State* L);
	static int _LUA_eq(lua_State* L);
	static int _LUA_index(lua_State* L);
	static int _LUA_tostring(lua_State* L);

};

class VOFOG_API VofogEntity{
public:
	std::shared_ptr< VofogTransformComponent> transform;
	VofogSpriteComponent sprite;
	std::string name;
	VofogEntity();
	VofogEntity(const char* nam) :name(nam) { transform = std::make_shared<VofogTransformComponent>(); };
	~VofogEntity();
	void moveEntity(float x, float y);
	void drawEntity();
	const char* getName() { return name.c_str(); }

	std::shared_ptr<VofogTransformComponent>& get_transform() { return transform; }
	void set_transform(std::shared_ptr<VofogTransformComponent> comp) { transform = comp; }

	void attachComponent(VofogTransformComponent newComp) { *transform = newComp; }



	static int _LUA_CreateEntity(lua_State* L);
	static int _LUA_AttachComponent(lua_State* L);
	static int _LUA_MoveEntity(lua_State* L) {
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -3); assert(luaEntity);
		float yMovement = lua_tonumber(L, -2);
		float xMovement = lua_tonumber(L, -1);
		luaEntity->moveEntity(xMovement, yMovement);
		return 0;
	}

	static int _LUA_SetTransformPosition(lua_State* L) {
		
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -3); assert(luaEntity);
		float yMovement = lua_tonumber(L, -2);
		float xMovement = lua_tonumber(L, -1);
		//luaEntity->transform->position = glm::vec2(xMovement, yMovement);
		return 0;
	}

	static int _LUA_DrawEntity(lua_State* L) {
		//	VOFOG_CORE_DEBUG("Lua entity drawing called");
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity);
		luaEntity->drawEntity();
		return 0;
	}

	static int _LUA_gc(lua_State* L) {
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity);
		luaEntity->~VofogEntity();
		return 0;
	}

	static int _LUA_eq(lua_State* L) {
		VofogEntity* luaEntity1 = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity1);
		VofogEntity* luaEntity2 = (VofogEntity*)lua_touserdata(L, -2); assert(luaEntity2);
		//if (luaEntity1->transform->position == luaEntity2->transform->position && luaEntity1->sprite.fileName == luaEntity2->sprite.fileName) {
		//	lua_pushboolean(L, true);
		//}
		return 1;
	}

	static int _LUA_getX(lua_State* L) {
		VofogEntity* luaEntity1 = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity1);
		//lua_pushnumber(L, luaEntity1->transform->position.x);
		return 1;
	}

	static int _LUA_getY(lua_State* L) {
		VofogEntity* luaEntity1 = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity1);
		//lua_pushnumber(L, luaEntity1->transform->position.y);
		return 1;
	}
	
	static int _LUA_newindex(lua_State* L) {
		assert(lua_isstring(L, -1));
		VofogEntity* luaEntity1 = (VofogEntity*)lua_touserdata(L, -2); assert(luaEntity1);
		std::string index = lua_tostring(L, -1);

		if (index == "transform") {
			VOFOG_CORE_DEBUG("New index transform called");
			/*lua_getuservalue(L, 1);
			lua_pushstring(L, "transform");
			lua_getglobal(L, "TransformComponent");
			lua_settable(L, -3);*/
		}
		return 0;
	}
	static int _LUA_index(lua_State* L) {
		assert(lua_isstring(L, -1));
		VofogEntity* luaEntity1 = (VofogEntity*)lua_touserdata(L, -2); assert(luaEntity1);
		std::string index = lua_tostring(L, -1);
		VOFOG_CORE_LOG("Entity: Trying to find a method for \"{}\"", index);

		if (index == "transform") {
			VOFOG_CORE_LOG("searching in transform table");

			lua_getuservalue(L, 1);
			lua_pushvalue(L, 2);
			lua_gettable(L, -2);
			return 1;
		}
		else
			VOFOG_CORE_LOG("Failed to find a transform method, checking entity methods");
		lua_getglobal(L, "Entity");
		lua_pushstring(L, index.c_str());
		lua_rawget(L, -2);
		return 1;
	}
	static int _LUA_tostring(lua_State* L) {
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -1); assert(luaEntity);
	//	glm::vec2 position = luaEntity->transform->position;
	//	glm::vec3 rotation = luaEntity->transform->rotation;

	//	std::string tostringData = "[LUA] Entity:\n\n\tVofogSpriteComponent:\n\t\tFile name: " + std::string(luaEntity->sprite.fileName) +
		//	"\n\n\tVofogTransformComponent:\n\t\tPosition: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")\n\t\t" +
		//	"Rotation: (" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z) + ")\n";
		//lua_pushstring(L, tostringData.c_str());
		return 1;
	}


};

