#pragma once
#include <Vofog.h>
#include <Rendering/Window.h>
#include <tools/Layers/DefaultLayer.h>
#include <tools/Layers/Layer.h>
#include <string>
#include <vector>
#include <tools/Scenes/Scene.h>
#include <time.h>       /* time */

namespace Vofog{


	class VOFOG_API DefaultScene: public Scene{

	public:

		DefaultScene():Scene(){}
		DefaultScene(std::string sceneName):Scene(sceneName){}

		virtual void initialize(){

			Scene::initialize();

			std::shared_ptr<Layer> defaultLayer = std::make_shared<DefaultLayer>();
			defaultLayer ->init(Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT);
			defaultLayer ->setName("Default Layer");
			m_sceneLayers.push_back(defaultLayer);


		}

		virtual void update(){
			Scene::update();


		}
		virtual void render(){
			Scene::render();

		}
		virtual void unload(){

			VOFOG_CORE_DEBUG("Scene: {}\tUnloading", m_sceneName);
			//cache some shit upon reload

		}

		virtual void onDetach(){


		}

		virtual void onEvent(){
			Scene::onEvent();

		}

		void onLoad()override{
			glGenFramebuffers(1, &m_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
			glBindTextureUnit(m_frameTextureID, m_frameTexture.getTextureID());

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameTexture.getTextureID(), 0);


			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
				VOFOG_CORE_FATAL("{}:\tFrame buffer failed to complete", __FILENAME__);
				DebugBreak();
			}
		}

		template<class Archive>
		void save(Archive& ar)const{
			ar(cereal::base_class<Scene>(this));
		}

		template<class Archive>
		void load(Archive& ar){
			ar(cereal::base_class<Scene>(this));
			onLoad();

		}





	};


}
CEREAL_REGISTER_TYPE(Vofog::DefaultScene);

