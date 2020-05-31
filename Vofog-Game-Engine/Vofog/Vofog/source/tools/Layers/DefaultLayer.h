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
#include <Entity Component System/Systems/Application/ScriptingSystem.h>
#include <Entity Component System/Systems/Application/PhysicsSystem.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tools/Events/EventManager.h>
#include <Rendering/Window.h>
#include <Rendering/Renderers/BulletDebugRenderer.h>
#include <tools/Physics/PhysicsFactory.h>



#include <tools/Assets/ModelLoader.h>


#include "btBulletDynamicsCommon.h"
namespace Vofog{
	class VOFOG_API DefaultLayer: public Layer{
	private:


		bool directionBools[4];
		bool _moveCam = false, wd = false, ad = false, sd = false, dd = false;
		glm::vec2 _rotationVector = glm::vec2(0, 0);
		std::shared_ptr<Vofog::ShaderProgram> _debugShaderProgram;
		std::shared_ptr<btDiscreteDynamicsWorld> m_world;
		BulletDebugRenderer dRenderer;
		bool renderDebugMode = false;




		std::vector<float> tmpData;
	public:

		DefaultLayer():Layer(){}

		DefaultLayer(std::shared_ptr<ECS::EntityManager> entityManager);
		void init(GLfloat viewportWidth, GLfloat viewportHeight)override;
		void update()override;
		void render()override;
		void render(ShaderProgram& shader) override;
		void onAttach() override;
		void onDetach() override;
		void initOnLoad();
		void onEvent() override;
		
		
		template<class Archive>
		void save(Archive& ar)const{
			ar(cereal::base_class<Layer>(this), _debugShaderProgram);//add m_lights
		}

		template<class Archive>
		void load(Archive& ar){
			ar(cereal::base_class<Layer>(this), _debugShaderProgram);//add m_lights
			initOnLoad();
		}



		virtual ~DefaultLayer(){
		}



	};

}
CEREAL_REGISTER_TYPE(Vofog::DefaultLayer);