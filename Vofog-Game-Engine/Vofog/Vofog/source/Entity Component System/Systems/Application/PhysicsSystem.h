#pragma once

#include <Vofog.h>

#include <tools/Events/EventManager.h>

#include <Entity Component System/Systems/EntitySystem.h>
#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Components/PhysicsComponent.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Entity.h>


namespace Vofog{
	namespace ECS{

		class VOFOG_API PhysicsSystem: public EntitySystem{
		public:
			PhysicsSystem(){}
			void update(EntityManager& manager)override;

			virtual ~PhysicsSystem(){}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<EntitySystem>(this));
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<EntitySystem>(this));
			}
		};
	}

}
CEREAL_REGISTER_TYPE(Vofog::ECS::PhysicsSystem);
