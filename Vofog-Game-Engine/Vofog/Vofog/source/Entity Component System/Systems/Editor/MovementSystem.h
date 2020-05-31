#pragma once
#include <Vofog.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Systems/EntitySystem.h>


namespace Vofog{

	namespace ECS{
		class VOFOG_API MovementSystem: public EntitySystem{

		public:

			MovementSystem(){}
			void update(EntityManager& manager)override;

			virtual ~MovementSystem(){}

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

}CEREAL_REGISTER_TYPE(Vofog::ECS::MovementSystem);