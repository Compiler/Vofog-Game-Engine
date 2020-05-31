#pragma once
#include <Vofog.h>
#include <sol/sol.hpp>

#include <Entity Component System/Entity.h>
#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Components/ScriptComponent.h>
namespace Vofog {
	namespace LuaState {
		extern sol::state* lua;

		extern void initialize();
	}


}