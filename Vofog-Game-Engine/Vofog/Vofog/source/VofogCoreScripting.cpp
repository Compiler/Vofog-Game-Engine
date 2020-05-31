#include "vpch.h"
#include "VofogCoreScripting.h"




Vofog::VofogCoreScripting::VofogCoreScripting() {
}

void Vofog::VofogCoreScripting::initConfigurationOptions() {
	Vofog::Logging::init(_imguiConsole);
}




void Vofog::VofogCoreScripting::vfg_unit_test() {


	_manager = std::make_shared<VofogEntityManager>();

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);


	/* --------- Transform table ----------- */
	lua_newtable(L);
	int transformTableIndex = lua_gettop(L);
	lua_pushvalue(L, transformTableIndex);
	lua_setglobal(L, "TransformComponent");

	lua_pushcfunction(L, VofogTransformComponent::_LUA_CreateTransform);
	lua_setfield(L, -2, "create");
	lua_pushcfunction(L, VofogTransformComponent::_LUA_SetPosition);
	lua_setfield(L, -2, "setPosition");

	/* --------- Transform Meta table ----------- */
	luaL_newmetatable(L, "TransformComponentMetaTable");
	lua_pushcfunction(L, VofogTransformComponent::_LUA_gc);
	lua_setfield(L, -2, "__gc");
	lua_pushcfunction(L, VofogTransformComponent::_LUA_eq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, VofogTransformComponent::_LUA_tostring);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, VofogTransformComponent::_LUA_index);
	lua_setfield(L, -2, "__index");



	/* --------- Entity table --------- */
	lua_newtable(L);
	int entityTableIndex = lua_gettop(L);
	lua_pushvalue(L, entityTableIndex);
	lua_setglobal(L, "Entity");

	constexpr int NUM_OF_UPVALUES = 1;
	lua_pushlightuserdata(L, _manager.get());
	lua_pushcclosure(L, VofogEntity::_LUA_CreateEntity, NUM_OF_UPVALUES);
	//lua_pushcfunction(L, VofogEntity::_LUA_CreateEntity);
	lua_setfield(L, -2, "create");
	lua_pushcfunction(L, VofogEntity::_LUA_MoveEntity);
	lua_setfield(L, -2, "move");
	lua_pushcfunction(L, VofogEntity::_LUA_DrawEntity);
	lua_setfield(L, -2, "draw");
	lua_pushcfunction(L, VofogEntity::_LUA_AttachComponent);
	lua_setfield(L, -2, "attachComponent");

	//lua_getglobal(L, "TransformComponent");
	//lua_pushcfunction(L, VofogEntity::_LUA_SetTransformPosition);
	//lua_setfield(L, -2, "transform_setPosition");
	/* --------- Entity Meta table --------- */
	luaL_newmetatable(L, "EntityMetaTable");
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, VofogEntity::_LUA_gc);
	lua_settable(L, -3);
	lua_pushstring(L, "__eq");
	lua_pushcfunction(L, VofogEntity::_LUA_eq);
	lua_settable(L, -3);
	lua_pushstring(L, "__tostring");
	lua_pushcfunction(L, VofogEntity::_LUA_tostring);
	lua_settable(L, -3);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, VofogEntity::_LUA_index);
	lua_settable(L, -3);





	lua_newtable(L);
	lua_pushstring(L, "Position");

	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, 5);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, 3);
	lua_settable(L, -3);

	lua_settable(L, -3);
	lua_setglobal(L, "Vector");



	VOFOG_CORE_WARN("EntityManager Entities: {}", _manager->getCount());

	auto errorCode = luaL_dofile(L, (VOFOG_DEFAULT_IMAGE_FILEPATH + "../scripts/lua/TestScript.lua").c_str());
	if (errorCode != LUA_OK) VOFOG_CORE_ERROR("VofogError: {}", lua_tostring(L, -1));
	lua_getglobal(L, "entity");
	if (lua_isuserdata(L, -1)) {
		//VOFOG_CORE_LOG("Lua entity found");
		VofogEntity* luaEntity = (VofogEntity*)lua_touserdata(L, -1);
		//VOFOG_CORE_DEBUG("Position: ({}, {}), \t Filename: {}", luaEntity->position.x, luaEntity->position.y, luaEntity->filename);
	}

	VOFOG_CORE_WARN("EntityManager Entities: {}", _manager->getCount());

	lua_close(L);
	VOFOG_CORE_WARN("EntityManager Entities: {}", _manager->getCount());
	//std::this_thread::sleep_for(std::chrono::seconds(5));
//	system("pause");
	std::cin.get();
	std::exit(1);

}

#include <cereal/archives/binary.hpp>
#include <sstream>

void Vofog::VofogCoreScripting::init() {
	//test();
	Vofog::Logging::init(_imguiConsole);
	{
	
		sol::state lua;
		lua.open_libraries(sol::lib::base);
		sol::usertype<VofogTransformComponent> transform_type = lua.new_usertype<VofogTransformComponent>("TransformComponent", sol::constructors<VofogTransformComponent()>());
		transform_type["setPosition"] = &VofogTransformComponent::setPosition;
		transform_type["toString"] = &VofogTransformComponent::toString;

		sol::usertype<VofogEntity> entity_type = lua.new_usertype<VofogEntity>("Entity", sol::constructors<VofogEntity(), VofogEntity(const char*)>(),
			"transform", &VofogEntity::transform);
		//lua.new_usertype<VofogEntity>("Entity", "transform", sol::property(&VofogEntity::get_transform, &VofogEntity::set_transform));
		//entity_type.set("transform", sol::property(&VofogEntity::get_transform, &VofogEntity::set_transform));
		entity_type["move"] = &VofogEntity::moveEntity;
		entity_type["attachComponent"] = &VofogEntity::attachComponent;
		entity_type["getName"] = &VofogEntity::getName;
		lua.script("print('bark bark bark!')");
		lua.script_file((VOFOG_DEFAULT_IMAGE_FILEPATH + "../scripts/lua/SolTestScript.lua").c_str());
		std::cin.get();
		std::exit(1);

	}
	//vfg_unit_test();
	//initConfigurationOptions();



	//TextureHandler::_init();
	////ScriptHandler::populate_all_scripts();
	//TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "cheapbricks_diffuse.jpg", "SRGB bricks", GL_SRGB, GL_RGB);//srgb
	//TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "container.png", "SRGBA container", GL_SRGB_ALPHA, GL_RGBA);//srgba

	char c;


}










void Vofog::VofogCoreScripting::render() {



}


void Vofog::VofogCoreScripting::onEvent() {


}
void Vofog::VofogCoreScripting::update() {



	EventManager::getInstance().clear();


}








