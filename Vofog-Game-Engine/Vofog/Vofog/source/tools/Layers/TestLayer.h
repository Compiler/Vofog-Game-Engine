#pragma once

#include <Vofog.h>
#include <tools/Layers/Layer.h>
#include <imgui/imgui.h>

#include <Rendering/Cameras/OrthographicCamera.h>
#include <Rendering/Cameras/PerspectiveCamera.h>
#include <Rendering/Renderables/Cube.h>
#include <Rendering/Renderables/Sprite2D.h>
#include <tools/Events/EventKey.h>
#include <tools/Events/EventMouse.h>
#include <Rendering/Renderables/ShaderAbstractions/PointLight.h>
#include <Rendering/TextureHandler.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/RenderableComponent.h>
#include <Rendering/Renderers/MeshRenderer.h>
#include <Entity Component System/Components/TransformComponent.h>
#include <Entity Component System/Components/PhysicsComponent.h>

#include <Entity Component System/Systems/Editor/MovementSystem.h>
#include <Entity Component System/Systems/Editor/ObjectPickingSystem.h>
#include <Entity Component System/Systems/Application/PhysicsSystem.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tools/Events/EventManager.h>
#include <Rendering/Window.h>
#include <Rendering/Renderers/BulletDebugRenderer.h>
#include <tools/Physics/PhysicsFactory.h>

#include "btBulletDynamicsCommon.h"
namespace Vofog{
	class VOFOG_API TestLayer: public Layer{
		private:

			ECS::Entity _testEntity1, _testEntity2, _lightEntity;
			Vofog::Renderer::MeshRenderer renderer;

			glm::vec3 _lightPosition = glm::vec3(-2, 3, 0);

			bool _moveCam = false, wd = false, ad = false, sd = false, dd = false;

			glm::vec2 _rotationVector = glm::vec2(0,0);


			std::shared_ptr<Vofog::ShaderProgram> _debugShaderProgram;

			void spawnBox(glm::vec3 pos, glm::vec3 size, float mass);

			std::shared_ptr<btDiscreteDynamicsWorld> world;
			/*btDispatcher*				dispatcher;
			btCollisionConfiguration*	collisionConfig;
			btBroadphaseInterface*		broadPhase;
			btConstraintSolver*			solver;*/

			BulletDebugRenderer dRenderer;

			bool renderDebugMode = false;
		public:

			TestLayer():Layer(){ m_entityManager = std::make_shared<ECS::EntityManager>(); }

			TestLayer(std::shared_ptr<ECS::EntityManager> entityManager);
			void tmpAddPlane(ECS::Entity entity, ECS::TransformComponent physicsBody);
			void tmpAddPlane(ECS::EntityID entity, ECS::TransformComponent physicsBody);
			void test_physics();
			void updatePhysics();

			void init(GLfloat viewportWidth, GLfloat viewportHeight)override;

			void update()override;
			void render()override;
			void render(ShaderProgram& shader) override;

			void onAttach() override;

			void onDetach() override;


			void initOnLoad();

			template<class Archive>
			void save(Archive& ar)const{
				ar(cereal::base_class<Layer>(this), _testEntity1, _testEntity2, _lightEntity, _debugShaderProgram);//add m_lights
				/*std::unique_ptr<btDefaultSerializer> sz = std::make_unique<btDefaultSerializer>();
				world->serialize(sz.get());
				FILE* file = fopen("testfile.bullet", "wb");
				fwrite(sz->getBufferPointer(), sz->getCurrentBufferSize(), 1, file);
				fclose(file);*/
			}

			template<class Archive>
			void load(Archive& ar){
				ar(cereal::base_class<Layer>(this), _testEntity1, _testEntity2, _lightEntity, _debugShaderProgram);//add m_lights
				initOnLoad();
			}



			virtual ~TestLayer(){
				world.reset();
			}


			void onEvent() override;

	};
	
}
CEREAL_REGISTER_TYPE(Vofog::TestLayer);