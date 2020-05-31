#include "vpch.h"
#include "PhysicsSystem.h"

namespace Vofog{

	namespace ECS{


		void PhysicsSystem::update(EntityManager& manager){
			static std::vector<EntityID> ids;
			ids = manager.getAssociatedEntities<TransformComponent, PhysicsComponent>();
			manager.getWorld()->stepSimulation(1.0f / 500.0f);
			for(int i = 0; i < ids.size(); i++){
				auto transform = manager.getComponent<TransformComponent>(ids[i]);
				auto body = manager.getComponent<PhysicsComponent>(ids[i])->body;
				if(!body)continue;
				btTransform trans;
				if(body && body->getMotionState()){
					body->getMotionState()->getWorldTransform(trans);
				} else{
					trans = body->getWorldTransform();
				}
				auto origin = trans.getOrigin();
				glm::vec3 pos = glm::vec3(origin.getX(), origin.getY(), origin.getZ());
				btScalar x, y, z;
				trans.getRotation().getEulerZYX(z, y, x);
				glm::vec3 rot(glm::degrees(x), glm::degrees(y), glm::degrees(z));
				transform->position = pos;
				transform->rotation = rot;
			}
		}
	}
}

