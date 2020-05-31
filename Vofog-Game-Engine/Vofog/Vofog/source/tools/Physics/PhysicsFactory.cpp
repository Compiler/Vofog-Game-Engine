#include <vpch.h>
#include "PhysicsFactory.h"

namespace Vofog{



	//attach plane to physics component
	void Vofog::Physics::attachPlane(ECS::EntityID entity, ECS::TransformComponent physicsBody, std::shared_ptr<btDiscreteDynamicsWorld> world, glm::vec3 normal, std::shared_ptr<ECS::EntityManager> m_entityManager){
		auto physicsComponent = m_entityManager->getComponent<ECS::PhysicsComponent>(entity);
		if(!physicsComponent->editor_planeBody){
			ECS::EntityID entityID = entity;
			std::shared_ptr<btStaticPlaneShape> planeShape = std::make_shared<btStaticPlaneShape>(btVector3(normal.x, normal.y, normal.z), 0);
			btTransform boxWorldTransform;
			boxWorldTransform.setIdentity();
			boxWorldTransform.setOrigin(btVector3(physicsBody.position.x, physicsBody.position.y, physicsBody.position.z));
			btMotionState* boxMotionState = new btDefaultMotionState(boxWorldTransform);
			btVector3 inertia(0, 0, 0);

			btRigidBody::btRigidBodyConstructionInfo info(0, boxMotionState, planeShape.get(), inertia);
			std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
			body->setUserPointer((void*)entityID);
			body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CO_GHOST_OBJECT);
			body->setActivationState(DISABLE_SIMULATION);
			world->addRigidBody(body.get(), btBroadphaseProxy::CollisionFilterGroups::SensorTrigger, btBroadphaseProxy::CollisionFilterGroups::DefaultFilter);
			physicsComponent->editor_planeBody = std::move(body);
			physicsComponent->editor_planeShape = std::move(planeShape);
		}
	}


	void Vofog::Physics::initiateWorld(std::shared_ptr<btDiscreteDynamicsWorld>& world){
		btCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
		btDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
		btBroadphaseInterface* broadPhase = new btDbvtBroadphase();
		btConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
		world = std::make_shared<btDiscreteDynamicsWorld>(dispatcher, broadPhase, solver, collisionConfig);
		world->setGravity(btVector3(0, -9.81f, 0));
		//world->setDebugDrawer((btIDebugDraw*)&dRenderer);
	}

	void Vofog::Physics::addBox(ECS::Entity entity, ECS::TransformComponent physicsBody, float mass, std::shared_ptr<btDiscreteDynamicsWorld> world, std::shared_ptr<ECS::EntityManager> m_entityManager){
		ECS::EntityID entityID = entity.getID();
		btBoxShape* btBoxCollisionShape = new btBoxShape(btVector3(physicsBody.size.x / 2.0f, physicsBody.size.y / 2.0f, physicsBody.size.z / 2.0f));
		btTransform boxWorldTransform;
		boxWorldTransform.setIdentity();
		boxWorldTransform.setOrigin(btVector3(physicsBody.position.x, physicsBody.position.y, physicsBody.position.z));
		btMotionState* boxMotionState = new btDefaultMotionState(boxWorldTransform);
		btVector3 inertia(0, 0, 0);
		btBroadphaseProxy::CollisionFilterGroups group = btBroadphaseProxy::CollisionFilterGroups::StaticFilter;
		if(mass != 0.0f){
			btBoxCollisionShape->calculateLocalInertia(mass, inertia);
			group = btBroadphaseProxy::CollisionFilterGroups::KinematicFilter;
		}
		std::shared_ptr<btMotionState> boxMotionStatePtr = std::make_shared<btDefaultMotionState>(boxWorldTransform);
		btRigidBody::btRigidBodyConstructionInfo info(mass, boxMotionStatePtr.get(), btBoxCollisionShape, inertia);

		std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
		body->setUserPointer((void*)entityID);
		VOFOG_CORE_DEBUG("{}", body->getCollisionFlags());
		world->addRigidBody(body.get(), group, btBroadphaseProxy::CollisionFilterGroups::AllFilter);
		VOFOG_CORE_WARN("{}", body->getCollisionFlags());

		Vofog::ECS::PhysicsComponent physicsComp; physicsComp.body = std::move(body); physicsComp.motion = boxMotionStatePtr; physicsComp.mass = mass;
		m_entityManager->attachComponent(entity, physicsComp);

	}

	void Vofog::Physics::addBox(ECS::EntityID entity, ECS::TransformComponent physicsBody, float mass, std::shared_ptr<btDiscreteDynamicsWorld> world, std::shared_ptr<ECS::EntityManager> m_entityManager){
		ECS::EntityID entityID = entity;
		btBoxShape* btBoxCollisionShape = new btBoxShape(btVector3(physicsBody.size.x / 2.0f, physicsBody.size.y / 2.0f, physicsBody.size.z / 2.0f));
		btTransform boxWorldTransform;
		boxWorldTransform.setIdentity();
		boxWorldTransform.setOrigin(btVector3(physicsBody.position.x, physicsBody.position.y, physicsBody.position.z));
		btMotionState* boxMotionState = new btDefaultMotionState(boxWorldTransform);
		btVector3 inertia(0, 0, 0);
		btBroadphaseProxy::CollisionFilterGroups group = btBroadphaseProxy::CollisionFilterGroups::StaticFilter;
		if(mass != 0.0f){
			btBoxCollisionShape->calculateLocalInertia(mass, inertia);
			group = btBroadphaseProxy::CollisionFilterGroups::KinematicFilter;
		}
		std::shared_ptr <btMotionState> boxMotionStatePtr = std::make_shared<btDefaultMotionState>(boxWorldTransform);
		btRigidBody::btRigidBodyConstructionInfo info(mass, boxMotionStatePtr.get(), btBoxCollisionShape, inertia);

		std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
		body->setUserPointer((void*)entityID);
		VOFOG_CORE_DEBUG("{}", body->getCollisionFlags());
		world->addRigidBody(body.get(), group, btBroadphaseProxy::CollisionFilterGroups::AllFilter);
		VOFOG_CORE_WARN("{}", body->getCollisionFlags());

		Vofog::ECS::PhysicsComponent physicsComp; physicsComp.body = std::move(body); physicsComp.motion = boxMotionStatePtr; physicsComp.mass = mass;

		m_entityManager->attachComponent(entity, physicsComp);

	}

	/*Add a physics component to the entity with physicsBody transform and mass to the given btDiscreteDynamicsWorld and EntityManager*/
	void Vofog::Physics::addPlane(ECS::Entity entity, ECS::TransformComponent physicsBody, glm::vec3 normal, btDiscreteDynamicsWorld* world, std::shared_ptr<ECS::EntityManager> m_entityManager){
		ECS::EntityID entityID = entity.getID();
		btStaticPlaneShape* btBoxCollisionShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0);
		btTransform boxWorldTransform;
		boxWorldTransform.setIdentity();
		boxWorldTransform.setOrigin(btVector3(physicsBody.position.x, physicsBody.position.y, physicsBody.position.z));
		btMotionState* boxMotionState = new btDefaultMotionState(boxWorldTransform);
		btVector3 inertia(0, 0, 0);

		std::shared_ptr<btMotionState> boxMotionStatePtr = std::make_shared<btDefaultMotionState>(boxWorldTransform);
		btRigidBody::btRigidBodyConstructionInfo info(0, boxMotionStatePtr.get(), btBoxCollisionShape, inertia);
		std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
		world->addRigidBody(body.get());
		body->setUserPointer((void*)entityID);
		Vofog::ECS::PhysicsComponent physicsComp; physicsComp.body = std::move(body); physicsComp.motion = boxMotionStatePtr;

		m_entityManager->attachComponent(entity, physicsComp);

	}


	void Vofog::Physics::moveBodyTransform(ECS::EntityID entity, std::shared_ptr<ECS::EntityManager> m_entityManager, glm::vec3 planePosition){
		auto physicsComp = m_entityManager->getComponent<ECS::PhysicsComponent>(entity);
		auto transComp = m_entityManager->getComponent<ECS::TransformComponent>(entity);
		btTransform init;
		init = physicsComp->body->getWorldTransform();
		init.setOrigin(btVector3(transComp->position.x, transComp->position.y, transComp->position.z));
		physicsComp->body->setWorldTransform(init);
		physicsComp->body->getMotionState()->setWorldTransform(init);
		physicsComp->body->activate(true);


		btTransform planeTransform;
		planeTransform = physicsComp->editor_planeBody->getCenterOfMassTransform();				//fix this line V
		//planeTransform.setOrigin(btVector3(transComp->position.x, transComp->position.y, transComp->position.z + transComp->size.z / 2.0f));
		planeTransform.setOrigin(btVector3(planePosition.x, planePosition.y, planePosition.z));

		physicsComp->editor_planeBody->setWorldTransform(planeTransform);
		physicsComp->editor_planeBody->getMotionState()->setWorldTransform(planeTransform);
		physicsComp->editor_planeBody->activate(true);
	}

}