#pragma once
#include <vpch.h>
#include "MovementSystem.h"
#include <Entity Component System/Systems/EntitySystem.h>
#include <Entity Component System/EntityManager.h>
#include <tools/Events/EventKey.h>
#include <tools/Events/EventManager.h>
namespace Vofog{
	namespace ECS{

		void MovementSystem::update(EntityManager& manager){

			static std::vector<EntityID> ids;
			ids = manager.getAssociatedEntities<TransformComponent, MovementComponent>();

			for(int i = 0; i < ids.size(); i++){

				std::shared_ptr<TransformComponent> transform = manager.getComponent<TransformComponent>(ids[i]);
				std::shared_ptr<MovementComponent> movement = manager.getComponent<MovementComponent>(ids[i]);
				std::shared_ptr<PhysicsComponent> physics = manager.getComponent<PhysicsComponent>(ids[i]);
				for(int k = 0; k < movement->movements.size(); k++){
					if(EventManager::getInstance().isKeyPressed(movement->movements[k].key))
						movement->movementFlags[k] = true;


					if(EventManager::getInstance().isKeyReleased(movement->movements[k].key))
						movement->movementFlags[k] = false;

					if(movement->movementFlags[k])
						transform->position += movement->movements[k].movement;
					static int count = 0;
					if(physics){
						btTransform trans;
						trans = physics->body->getWorldTransform();
						glm::vec3 physicsPosition = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
						if(physicsPosition != transform->position && count >= 1000){
							trans.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
							physics->body->setWorldTransform(trans);
							count = 0;
						}
						count++;
					}
				}


			}

		}

	}

}