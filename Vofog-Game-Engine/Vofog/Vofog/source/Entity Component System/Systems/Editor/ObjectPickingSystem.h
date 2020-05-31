#pragma once
#include <Vofog.h>
#include <tools/Logging.h>
#include <tools/Events/EventManager.h>
#include <Rendering/Window.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Components/PhysicsComponent.h>

#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Systems/EntitySystem.h>


#include <tools/Physics/PhysicsFactory.h>
#include <Rendering/Cameras/PerspectiveCamera.h>

namespace Vofog{

	namespace ECS{
		class VOFOG_API ObjectPickingSystem: public EntitySystem{

		private:
			std::shared_ptr<Vofog::Camera> _camera;

		public:
			ObjectPickingSystem(std::shared_ptr<Vofog::Camera> camera): _camera(camera){}
			ObjectPickingSystem(){}
			void setCamera(std::shared_ptr<Vofog::Camera> camera){ _camera = camera; }
			void update(EntityManager& manager)override;

			virtual ~ObjectPickingSystem(){}

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<EntitySystem>(this), _camera);
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<EntitySystem>(this), _camera);
			}
		};

	}

}
CEREAL_REGISTER_TYPE(Vofog::ECS::ObjectPickingSystem);