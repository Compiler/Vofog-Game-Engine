#include <vpch.h>
#include "DefaultLayer.h"

namespace Vofog{

	DefaultLayer::DefaultLayer(std::shared_ptr<ECS::EntityManager> entityManager):Layer(entityManager){
		m_entityManager = entityManager;
		VOFOG_CORE_LOG("DefaultLayer:\tConstructor(std::shared_ptr<ECS::EntityManager> entityManager) called");
	}

	void DefaultLayer::init(GLfloat viewportWidth, GLfloat viewportHeight){
		Layer::init(viewportWidth, viewportHeight);
		
		m_camera->setCameraPosition(glm::vec3(0, 0, 3));
		_debugShaderProgram = std::make_shared<ShaderProgram>("DEBUG//DebugPicking.vert", "DEBUG//DebugPicking.frag", "DEBUG//DebugPicking.geom");
		Physics::initiateWorld(m_world);
		m_entityManager->setWorld(m_world);

		VOFOG_CORE_LOG("DefaultLayer:\init(GLfloat viewportWidth, GLfloat viewportHeight) called");
		
		m_entityManager->addSystem(std::make_shared<ECS::PhysicsSystem>(), ECS::EngineStateTuple::APPLICATION);
		m_entityManager->addSystem(std::make_shared<ECS::ObjectPickingSystem>(m_camera), ECS::EngineStateTuple::EDITOR);
		m_entityManager->addSystem(std::make_shared<ECS::ScriptingSystem>(), ECS::EngineStateTuple::APPLICATION);
		ECS::Entity defaultEntity;
		m_entityManager->addEntity(defaultEntity);

		Vofog::ECS::TransformComponent transform; transform.position = glm::vec3(0.0, 0.0, -3.0); transform.size = glm::vec3(1, 1, 1);
		Vofog::ECS::RenderableComponent renderable;
		Vofog::ECS::MeshRendererComponent renderer; renderable.color = glm::vec4(0.2, 0.3, 0.2, 1);
		Vofog::ECS::MeshTypeComponent meshType;  meshType.meshType = Vofog::ECS::MeshType::CUBE;
		Vofog::ECS::MovementComponent movementComponent;
		Physics::addBox(defaultEntity.getID(), transform, 1, m_world, m_entityManager);
		Physics::attachPlane(defaultEntity.getID(), transform, m_world, m_camera->getDirection(), m_entityManager);
		

		m_entityManager->attachComponent(defaultEntity, transform, renderer, meshType, renderable);


	//	tmpData = ModelLoader::loadModel(VOFOG_DEFAULT_IMAGE_FILEPATH + "..//models//donut_icing.obj");
		/*std::string data = "";
		for(int i = 0; i < tmpData.size(); i++){
			if(i % 8 == 0) data += "\n";
			data += " " + std::to_string(tmpData[i]);
		}*/
		VOFOG_CORE_LOG("{}",tmpData.size());

	//	btConvexHullShape* shape = new btConvexHullShape();
	}

	void DefaultLayer::update(){
		Layer::update();
		//m_world->setGravity(btVector3(0,-0.05,0));
		m_camera->update();


		glm::vec3 dir(0.0f);
		//glm::vec3(m_entityManager->getComponent<ECS::TransformComponent>(ids[0])->rotation = glm::vec3(0));
		
		
		
		if (EventManager::isKeyPressed(VofogKeyStore::VOFOG_KEY_W))wd = true; if (EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_W))wd = false;
		if (EventManager::isKeyPressed(VofogKeyStore::VOFOG_KEY_A))ad = true; if (EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_A))ad = false;
		if (EventManager::isKeyPressed(VofogKeyStore::VOFOG_KEY_S))sd = true; if (EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_S))sd = false;
		if (EventManager::isKeyPressed(VofogKeyStore::VOFOG_KEY_D))dd = true; if (EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_D))dd = false;
		if (EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_SPACE))renderDebugMode = !renderDebugMode;

		if(wd)dir.y += 1;
		if(ad)dir.x -= 1;
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

	void DefaultLayer::render(){
		m_shaderProgram->use();
		m_shaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
		m_shaderProgram->setUniformVec3("lightPos", 0, 0, 2);
		m_shaderProgram->setUniformVec3("lightColor", 1, 1, 1);
		m_shaderProgram->setUniformVec3("u_viewPos", m_camera->getPosition());

		if(!renderDebugMode){
			m_meshRenderer->begin();
			m_meshRenderer->submit_mesh_test(m_entityManager);
			//m_meshRenderer->submit_raw_model(tmpData, glm::vec3(0,0,-2));
			m_meshRenderer->end();
			m_meshRenderer->flush();
		}
		_debugShaderProgram->use();
		_debugShaderProgram->setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);

		m_meshRenderer->begin();
		m_meshRenderer->submit_debug(m_entityManager);
		m_meshRenderer->end();
		m_meshRenderer->flush();


		if (renderDebugMode) {
			dRenderer.getShader().use();
			dRenderer.getShader().setUniformMat4("u_vertexMVP", &m_camera->getCombinedMatrix()[0][0]);
			m_world->debugDrawWorld();
		}
	}

	void DefaultLayer::render(ShaderProgram& shader) {}

	void DefaultLayer::onAttach() {}

	void DefaultLayer::onDetach() {}
	
	void DefaultLayer::initOnLoad(){
		Physics::initiateWorld(m_world);
		m_entityManager->setWorld(m_world);
		m_meshRenderer = std::make_shared<Renderer::MeshRenderer>();
		m_meshRenderer->init();

		GLint textureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
		for (int i = 0; i < textureUnits; i++) {
			m_shaderProgram->setUniformInt1(std::string(std::string("u_textures[") + std::to_string(i) + "]").c_str(), i);
		}

		dRenderer = BulletDebugRenderer(); m_world->setDebugDrawer((btIDebugDraw*)&dRenderer);
		//test_physics();
		auto es = m_entityManager->getAssociatedEntities<ECS::PhysicsComponent, ECS::TransformComponent>();
		for(int i = 0; i < es.size(); i++){
			auto transform = m_entityManager->getComponent<ECS::TransformComponent>(es[i]);
			float mass = m_entityManager->getComponent<ECS::PhysicsComponent>(es[i])->mass;
			VOFOG_CORE_LOG("Adding body with mass {}", mass);
			Vofog::Physics::addBox(es[i], *transform, mass, m_world, m_entityManager);
			Physics::attachPlane(es[i], *transform, m_world, m_camera->getDirection(), m_entityManager);



		}
	}
	
	void DefaultLayer::onEvent(){
		
		
		static bool mouseButtonDown = false;
		static double xpos = 0;
		static double ypos = 0;
		//if(event.isHandled()) return;
		static float lastX, lastY;
		//if(event.getType() == Event::EventType::MouseMoved){
		//	static bool firstRun = true;
		//	GLfloat sen = 0.5f;
		//	const Vofog::EventMouseMoved& childEvent = static_cast<const Vofog::EventMouseMoved&>(event);
		//	xpos = childEvent.getX();
		//	ypos = childEvent.getY();


		//	//double xx = xpos - (_viewportWidth / 2);
		//	double xx = xpos - lastX;
		//	//double yy = -ypos + (_viewportHeight / 2);
		//	double yy = lastY - ypos;
		//	if(firstRun){
		//		lastX = xx;
		//		lastY = yy;
		//		firstRun = false;
		//	}
		//	lastX = xpos;
		//	lastY = ypos;
		//	_rotationVector.x = xx * 50 * ImGui::GetIO().DeltaTime;
		//	_rotationVector.y = yy * 50 * ImGui::GetIO().DeltaTime;


		//}
		


		
	
		




	}

}