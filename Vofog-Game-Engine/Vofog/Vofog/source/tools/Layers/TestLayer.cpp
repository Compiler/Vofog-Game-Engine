#include <vpch.h>
#include "TestLayer.h"


namespace Vofog{
	TestLayer::TestLayer(std::shared_ptr<ECS::EntityManager> entityManager): Layer(entityManager){
		m_meshRenderer = std::make_shared<Vofog::Renderer::MeshRenderer>(); m_meshRenderer->init();
	}

	void TestLayer::init(GLfloat viewportWidth, GLfloat viewportHeight){
		renderer.init();
		dRenderer = BulletDebugRenderer();
		test_physics();
		m_entityManager->setWorld(world);
		_rotationVector.x = 0;
		_rotationVector.y = 0;
		
		GLfloat _viewportWidth = viewportWidth; GLfloat _viewportHeight = viewportHeight;
		
		if(viewportWidth <= 1) _viewportWidth = Vofog::Window::SCREEN_WIDTH;
		if(viewportHeight <= 1) _viewportHeight = Vofog::Window::SCREEN_HEIGHT;

		m_camera = std::make_shared<Vofog::PerspectiveCamera>(45, _viewportWidth, _viewportHeight);
		m_camera->setCameraPosition(glm::vec3(0, 0, 3));
		
		_debugShaderProgram = std::make_shared<ShaderProgram>("DEBUG//DebugPicking.vert", "DEBUG//DebugPicking.frag", "DEBUG//DebugPicking.geom");
		_debugShaderProgram->use();
		_debugShaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);


		m_shaderProgram = std::make_shared<ShaderProgram>("pass.vert", "pass.frag", "");
		m_shaderProgram->use();
		m_shaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
		_testEntity2.setName("Static Cube");
		_lightEntity.setName("Light");
		_testEntity1.setName("Dynamic Cube");
		ECS::Entity e = ECS::Entity("Ground");
		m_entityManager->addSystem(std::make_shared<ECS::MovementSystem>(), ECS::EngineStateTuple::EDITOR);
		m_entityManager->addSystem(std::make_shared<ECS::PhysicsSystem>(), ECS::EngineStateTuple::APPLICATION);
		m_entityManager->addSystem(std::make_shared<ECS::ObjectPickingSystem>(m_camera), ECS::EngineStateTuple::EDITOR);
		m_entityManager->addEntity(_testEntity1);
	//	m_entityManager->addEntity(_testEntity2);
		m_entityManager->addEntity(_lightEntity);
		m_entityManager->addEntity(e);
		Vofog::ECS::TransformComponent transform; transform.position = glm::vec3(0, -0.5, -5); transform.size = glm::vec3(1, 1, 1);
		Vofog::ECS::RenderableComponent renderable; 
		Vofog::ECS::SpriteComponent sprite;  //sprite.texture = *TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "cheapbricks_diffuse.jpg");
		Vofog::ECS::MeshRendererComponent renderer; renderable.color = glm::vec4(0.2,0.3, 0.2,1);
		Vofog::ECS::MeshTypeComponent meshType;  meshType.meshType = Vofog::ECS::MeshType::CUBE;
		Vofog::ECS::MovementComponent movementComponent;
		float mag = 0.01f;
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_LEFT, glm::vec3(-mag, 0, 0)));
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_RIGHT, glm::vec3(mag, 0, 0)));
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_UP, glm::vec3(0, mag, 0)));
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_DOWN, glm::vec3(0, -mag, 0)));
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_MINUS, glm::vec3(0, 0, -mag)));
		movementComponent.movements.push_back(ECS::MovementComponentData(Vofog::VofogKeyStore::VOFOG_KEY_EQUAL, glm::vec3(0, 0, mag)));
		for(int i = 0; i < movementComponent.movements.size(); i++)movementComponent.movementFlags.push_back(false);

		m_entityManager->attachComponent(_testEntity1,
			transform,
			renderer, meshType);
		m_entityManager->attachComponent(_testEntity1.getID(), renderable);
		//spawnBox(glm::vec3(0, -0.5, -4), glm::vec3(1, 1, 1), 0.0f);

		Physics::addBox(_testEntity1.getID(), transform, 1, world, m_entityManager);
		tmpAddPlane(_testEntity1.getID(), transform);
		renderable.color = glm::vec4(0.2, 0.2, 0.3, 1);
		transform.position = _lightPosition; transform.size = glm::vec3(0.2);
		m_entityManager->attachComponent(_lightEntity,
			transform, movementComponent,
			renderer, meshType);

		transform.position = glm::vec3(0, -2, -5); transform.size = glm::vec3(2, 1, 1);
		transform.rotation = glm::vec3(0, 0, 0);
		meshType.meshType = Vofog::ECS::MeshType::CUBE;

	/*	m_entityManager->attachComponent(_testEntity2,
			transform, renderable,
			renderer, meshType);
		sprite.texture = *TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "cheapbricks_diffuse.jpg");

		Physics::addBox(_testEntity2, transform, 0, (btDiscreteDynamicsWorld*)world, m_entityManager);*/
		renderable.color = glm::vec4(0.15, 0.0, 0.0, 1);
		transform.position = glm::vec3(-2, -4, -7); transform.size = glm::vec3(20, 1, 10);
		m_entityManager->attachComponent(e,
			transform, renderable,
			renderer, meshType);

		Physics::addBox(e, transform, 0, world, m_entityManager);
		tmpAddPlane(e, transform);
		m_shaderProgram->use();
		GLint textureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
		for(int i = 0; i < textureUnits; i++){
			m_shaderProgram->setUniformInt1(std::string(std::string("u_textures[") + std::to_string(i) + "]").c_str(), i);
		}

		glm::vec2 sz = glm::vec2(0.5f, 0.5f);
		static int scale = 5;
		for(int x = 0; x < scale; x++){
			for(int y = 0; y < scale; y++){
				for(int z = 0; z < scale; z++){
					float xp = -2 + (x * sz.x);
					float yp = 25 + (y * sz.y);
					float zp = -5 + (z * sz.y);
					//spawnBox(glm::vec3(xp, yp, zp), glm::vec3(sz.x, sz.y, sz.x), 0.5f);
				}
			}
		}
		//spawnBox(glm::vec3(-2, -3, -5), glm::vec3(1,1,1), 0);

	}


	

	void TestLayer::update(){
		if(EventManager::isMouseReleased(Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_MIDDLE)) renderDebugMode = !renderDebugMode;
		//if(m_entityManager->hasTransitioned())world->stepSimulation(1/60.0f);
		static int count = 0;
		count++;
		if(count == 1000){
			//VOFOG_CORE_FATAL("{} {} {}", bodies[0]->getWorldTransform().getOrigin().getX(), bodies[0]->getWorldTransform().getOrigin().getY(), bodies[0]->getWorldTransform().getOrigin().getZ());
			count= 0;
		}
		//updatePhysics();
		m_camera->resize(Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT);
		m_camera->update();


		glm::vec3 dir(0.0f);
		//glm::vec3(m_entityManager->getComponent<ECS::TransformComponent>(ids[0])->rotation = glm::vec3(0));

		if(wd)dir.y += 1;
		if(ad)dir.x += -1;
		if(sd)dir.y -= 1;
		if(dd)dir.x += 1;
		m_camera->move(dir);
		if(_moveCam){
			m_camera->rotate(glm::vec3(1, 0, 0), _rotationVector.x);
			m_camera->rotate(glm::vec3(0, 1, 0), _rotationVector.y);
		}
		_rotationVector.x = 0;
		_rotationVector.y = 0;
	}

	void TestLayer::render(){
		//VOFOG_CORE_LOG("{} {} {}", bodies[0]->getWorldTransform().getOrigin().getX(), bodies[0]->getWorldTransform().getOrigin().getY(), bodies[0]->getWorldTransform().getOrigin().getZ());

		{
			GLenum error = glGetError();
			if(error != GL_NO_ERROR){
				VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
				DebugBreak();
			}

		}


		
		m_shaderProgram->use();
		m_shaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
		std::shared_ptr<ECS::TransformComponent> tComp = m_entityManager->getComponent<ECS::TransformComponent>(_lightEntity.getID());
		m_shaderProgram->setUniformVec3("lightPos", tComp->position);
		m_shaderProgram->setUniformVec3("lightColor", 1, 1, 1);
		//glBindVertexArray(_vertexID);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//
		if(!renderDebugMode){
			renderer.begin();
			renderer.submit_mesh_test(m_entityManager);
			renderer.end();
			renderer.flush();
		}

		_debugShaderProgram->use();
		_debugShaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
		
		renderer.begin();
		renderer.submit_debug(m_entityManager);
		renderer.end();
		renderer.flush();

		float mag = 50;
	
		if(renderDebugMode){
		dRenderer.getShader().use();
		dRenderer.getShader().setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
			//world->getDebugDrawer()->drawLine(btVector3(beg.getX(), beg.getY(), beg.getZ() - 1), btVector3(end.getX(), end.getY(), end.getZ()), color);
			/*world->getDebugDrawer()->drawLine(btVector3(beg.getX(), beg.getY(), beg.getZ() - 1),
				btVector3(mouseClickPosition.x, mouseClickPosition.y, mouseClickPosition.z), btVector3(0, 0, 1));
			world->getDebugDrawer()->drawLine(btVector3(beg.getX(), beg.getY(), beg.getZ() - 1),
				btVector3(mouseDragPosition.x, mouseDragPosition.y, mouseDragPosition.z), btVector3(1, 0, 0));
			world->getDebugDrawer()->drawLine(btVector3(mouseClickPosition.x, mouseClickPosition.y, mouseClickPosition.z),
				btVector3(mouseDragPosition.x, mouseDragPosition.y, mouseDragPosition.z), btVector3(0, 1, 0));
			world->getDebugDrawer()->drawLine(btVector3(beg.getX(), beg.getY(), beg.getZ() - 1),
				btVector3(directionDrag.x, directionDrag.y, beg.getZ() - 1), btVector3(1, 0, 1));*/
			//if(entityGrabbed){
			//	auto currentEntity = m_entityManager->getCurrentEntity();
			//	auto currentEntityTransform = m_entityManager->getComponent<ECS::TransformComponent>(currentEntity);
			//	world->getDebugDrawer()->drawLine(btVector3(currentEntityTransform->position.x, currentEntityTransform->position.y, currentEntityTransform->position.z),
			//		btVector3(mouseDragPosition.x, mouseDragPosition.y, currentEntityTransform->position.z), btVector3(1, 1, 1));
			//}
			world->debugDrawWorld();
		}
		glBindTextureUnit(4, 5);
		
	}


	void TestLayer::render(ShaderProgram& shader){

	}

	void TestLayer::onAttach(){
		VOFOG_CORE_DEBUG("attached");
	}

	void TestLayer::onDetach(){
		VOFOG_CORE_DEBUG("detached");
	}

	void TestLayer::onEvent(){
		




	}

	void TestLayer::tmpAddPlane(ECS::Entity entity, ECS::TransformComponent physicsBody){
		auto physicsComponent = m_entityManager->getComponent<ECS::PhysicsComponent>(entity);
		if(!physicsComponent->editor_planeBody){
			ECS::EntityID entityID = entity.getID();
			auto normal = m_camera->getDirection();
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
			physicsComponent->editor_planeBody = std::move( body);
			physicsComponent->editor_planeShape = std::move(planeShape);
		}
	}

	void TestLayer::tmpAddPlane(ECS::EntityID entity, ECS::TransformComponent physicsBody){
		auto physicsComponent = m_entityManager->getComponent<ECS::PhysicsComponent>(entity);
		if(!physicsComponent->editor_planeBody){
			ECS::EntityID entityID = entity;
			auto normal = m_camera->getDirection();
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

	void TestLayer::test_physics(){
		btCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
		btDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
		btBroadphaseInterface* broadPhase = new btDbvtBroadphase();
		btConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
		world = std::make_shared<btDiscreteDynamicsWorld>(dispatcher, broadPhase, solver, collisionConfig);
		world->setGravity(btVector3(0, -9.81f, 0));
		world->setDebugDrawer((btIDebugDraw*)&dRenderer);

		/*btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, -2, 0));
		btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btMotionState* motion = new btDefaultMotionState(t);*/

		/*btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
		btRigidBody* body = new btRigidBody(info);
		world->addRigidBody(body);
		bodies.push_back(body);
		addBox();*/
	}

	void TestLayer::spawnBox(glm::vec3 pos, glm::vec3 size, float mass){
		ECS::Entity entity = ECS::Entity();
		m_entityManager->addEntity(entity);
		ECS::EntityID id = entity.getID();
		ECS::TransformComponent physicsBody; physicsBody.position = pos; physicsBody.size = size;
		m_entityManager->attachComponent<ECS::TransformComponent>(entity, physicsBody);
		btCompoundShape* shape = new btCompoundShape();
		btBoxShape* box = new btBoxShape(btVector3(physicsBody.size.x / 2.0f, physicsBody.size.y / 2.0f, physicsBody.size.z / 2.0f));
		btTransform t1;
		t1.setIdentity();
		t1.setOrigin(btVector3(physicsBody.position.x, physicsBody.position.y, physicsBody.position.z));
		btMotionState* motion = new btDefaultMotionState(t1);
		/*btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(-physicsBody.size.x / 2.0f, -physicsBody.size.y / 2.0f, -physicsBody.size.z / 2.0f));
		shape->addChildShape(t, box);*/

		btVector3 inertia(0, 0, 0);
		if(mass != 0.0f){
			box->calculateLocalInertia(mass, inertia);
		}
		std::shared_ptr<btMotionState> motionState = std::make_shared<btDefaultMotionState>(t1);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motionState.get(), box, inertia);

		// ========================consider using raw ptr for Body ========================//
		std::shared_ptr<btRigidBody> body = std::make_shared<btRigidBody>(info);
		world->addRigidBody(body.get());
		body->setUserPointer((void*)id);
		Vofog::ECS::PhysicsComponent physicsComp; physicsComp.body = std::move(body); physicsComp.motion = motionState;

		m_entityManager->attachComponent(entity, physicsComp);
		Vofog::ECS::RenderableComponent renderable;
		renderable.color = glm::vec4((float)(std::rand() %255) / 255.0f, (float)(std::rand() % 255) / 255.0f, (float)(std::rand() % 255) / 255.0f, 1);
		Vofog::ECS::MeshRendererComponent renderer;
		Vofog::ECS::MeshTypeComponent meshType;  meshType.meshType = Vofog::ECS::MeshType::CUBE;
		m_entityManager->attachComponent(entity, renderable, renderer, meshType);

		//	bodies.push_back(std::move(body));


	}
	void Vofog::TestLayer::initOnLoad(){
		renderer.init();
		dRenderer = BulletDebugRenderer();
		test_physics();
		m_entityManager->setWorld(world);
		m_entityManager->setCurrentEntity(_testEntity1.getID());
		auto es = m_entityManager->getAssociatedEntities<ECS::PhysicsComponent, ECS::TransformComponent>();
		for(int i = 0; i < es.size(); i++){
			auto transform = m_entityManager->getComponent<ECS::TransformComponent>(es[i]);
			auto physics = m_entityManager->getComponent<ECS::PhysicsComponent>(es[i]);
			float mass = physics->mass;
			VOFOG_CORE_LOG("Adding body with mass {}", mass);
			Vofog::Physics::addBox(es[i], *transform, mass, world, m_entityManager);
			tmpAddPlane(es[i], *transform);
		}
	}
}