#pragma once
#include <Vofog.h>
#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Entity.h>
#include <Entity Component System/Components/PhysicsComponent.h>
#include <Entity Component System/Components/TransformComponent.h>

#include "btBulletDynamicsCommon.h"
namespace Vofog{
	
	namespace Physics{


		extern void initiateWorld(std::shared_ptr<btDiscreteDynamicsWorld>& world);

		extern void attachPlane(ECS::EntityID entity, ECS::TransformComponent physicsBody, std::shared_ptr<btDiscreteDynamicsWorld> world, glm::vec3 normal, std::shared_ptr<ECS::EntityManager> m_entityManager);
		extern void addBox(ECS::Entity entity,   ECS::TransformComponent physicsBody, float mass, std::shared_ptr<btDiscreteDynamicsWorld> world, std::shared_ptr<ECS::EntityManager> m_entityManager);
		extern void addBox(ECS::EntityID entity, ECS::TransformComponent physicsBody, float mass, std::shared_ptr<btDiscreteDynamicsWorld> world, std::shared_ptr<ECS::EntityManager> m_entityManager);
		extern void addPlane(ECS::Entity entity, ECS::TransformComponent physicsBody, glm::vec3 normal, btDiscreteDynamicsWorld* world, std::shared_ptr<ECS::EntityManager> m_entityManager);
		extern void moveBodyTransform(ECS::EntityID entity, std::shared_ptr<ECS::EntityManager> m_entityManager, glm::vec3 planePosition);
	}

}