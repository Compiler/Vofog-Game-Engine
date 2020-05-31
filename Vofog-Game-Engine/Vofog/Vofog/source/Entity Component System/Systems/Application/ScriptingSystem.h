#pragma once

#include <Vofog.h>

#include <tools/Events/EventManager.h>

#include <Entity Component System/Systems/EntitySystem.h>
#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Components/PhysicsComponent.h>
#include <Entity Component System/Components/ScriptComponent.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Entity.h>

#include <tools/LuaState.h>

namespace Vofog {
	namespace ECS {

		class VOFOG_API ScriptingSystem : public EntitySystem {
			public:
			ScriptingSystem() {}
			void update(EntityManager& manager)override;

			virtual ~ScriptingSystem() {}

			template<class Archive>
			void save(Archive& ar)const {
				ar(cereal::base_class<EntitySystem>(this));
			}

			template<class Archive>
			void load(Archive& ar) {
				ar(cereal::base_class<EntitySystem>(this));
			}
		};
	}

}
CEREAL_REGISTER_TYPE(Vofog::ECS::ScriptingSystem);
