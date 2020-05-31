#pragma once
#include <Vofog.h>
#include <Rendering/Window.h>
#include <tools/Events/EventListener.h>
#include <tools/Events/EventMouse.h>
#include <vector>
#include <Rendering/ShaderProgram.h>
#include <Rendering/Renderables/Cube.h>
#include <Rendering/Renderers/MeshRenderer.h>
#include <Rendering/Cameras/Camera.h>

#include <Rendering/Cameras/PerspectiveCamera.h>
#include <Rendering/Renderables/ShaderAbstractions/Light.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Entity.h>
#include <Entity Component System/EntityManager.h>



namespace Vofog{

	class VOFOG_API Layer : public Vofog::EventListener{

	protected:

		std::shared_ptr<Vofog::ShaderProgram> m_shaderProgram;
		std::shared_ptr<Vofog::ECS::EntityManager> m_entityManager;

		std::shared_ptr<Vofog::Camera> m_camera;

		std::shared_ptr<Vofog::Renderer::MeshRenderer> m_meshRenderer;

		std::string m_layerName;

	public:
		VOFOG_SHARE_GUI_DATA;

		Layer();
		Layer(std::shared_ptr<ECS::EntityManager> entityManager);
		Layer(GLfloat viewportWidth, GLfloat viewportHeight);

		std::shared_ptr<Vofog::Camera> getCamera(){ return m_camera; }
		virtual void onEvent(){}

		void setShaderProgram(std::shared_ptr<Vofog::ShaderProgram> program){ m_shaderProgram = program;}


		const char* getLayerName() const {return m_layerName.c_str();}
		std::shared_ptr<Vofog::ShaderProgram> getShaderProgram() const { return m_shaderProgram; }

		virtual void init(GLfloat viewportWidth, GLfloat viewportHeight);

		virtual void onAttach()	=0;
		virtual void onDetach()	=0;
		virtual void update();
		virtual void render()	=0;
		virtual void onLoad();
		virtual void render(ShaderProgram& shader)	=0;


		void setName(std::string name){ m_layerName = name; }
		

		virtual ~Layer(){
			VOFOG_CORE_WARN("Deleting {} object", __FILENAME__);
		};


		
		template<class Archive>
		void save(Archive& ar)const{
			VOFOG_CORE_DEBUG("Saving layer: {}", m_layerName);
			ar(m_layerName, m_entityManager, m_camera, m_shaderProgram);//add m_lights
			VOFOG_CORE_DEBUG("Done saving layer: {}", m_layerName);

		}

		template<class Archive>
		void load(Archive& ar){
			
			VOFOG_CORE_DEBUG("Loading layer: {}", m_layerName);
			ar(m_layerName, m_entityManager, m_camera, m_shaderProgram);//add m_lights
			VOFOG_CORE_DEBUG("Done loading layer: {}", m_layerName);
			onLoad();
		}


		
		
		

	};
}