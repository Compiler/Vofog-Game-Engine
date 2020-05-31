#include <vpch.h>
#include "ScriptingSystem.h"

void Vofog::ECS::ScriptingSystem::update(EntityManager& manager) {
	static std::vector<EntityID> ids;
	ids = manager.getAssociatedEntities<ScriptComponent>();
	for (int i = 0; i < ids.size(); i++) {
		try {
			Vofog::LuaState::lua->script_file(manager.getComponent<ScriptComponent>(ids[i])->filepath);
			//sol::function initFunction	 = (*Vofog::LuaState::lua)["init"];
			//if (initFunction.valid()) VOFOG_CORE_LOG("Init exists");
			//else  VOFOG_CORE_LOG("Init doesnt exists");
			sol::function updateFunction = (*Vofog::LuaState::lua)["update"];
			(*Vofog::LuaState::lua)["thisEntity"] = manager.getEntity(ids[i]);
			//sol::function onExitFunction = (*Vofog::LuaState::lua)["onExit"];
			updateFunction();
		} catch (sol::error& err) {
			std::string error = err.what();
			error = error.substr(error.find_last_of('/') + 1);
			VOFOG_CORE_FATAL("[LUA] Error: {}", error);
		}
	}
}