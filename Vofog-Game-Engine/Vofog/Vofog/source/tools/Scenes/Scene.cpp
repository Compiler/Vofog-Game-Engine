#include <vpch.h>
#include "Scene.h"

namespace Vofog{

	int Scene::COUNT = 0;


	Scene::Scene(){ m_sceneName = "Scene #" + std::to_string(++COUNT); }
	Scene::Scene(std::string sceneName):m_sceneName(sceneName){}
	Scene::Scene(const Scene& old):m_sceneName(old.m_sceneName){
		if(!old.m_sceneLayers.empty())m_sceneLayers = old.m_sceneLayers;
	}
	void Scene::initialize(){
		if(m_sceneLoaded == true) return;
		m_sceneLoaded = true;
		VOFOG_CORE_DEBUG("Scene: {}\tinitializing", m_sceneName);
		m_frameTextureID = 13;


		m_frameTexture.init("frame texture", Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT, GL_RGBA16F, GL_RGBA);
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

		for(int i = 0; i < m_sceneLayers.size(); i++)
			m_sceneLayers[i]->init(0, 0);
	}


	void Scene::update(){
		for(int i = 0; i < m_sceneLayers.size(); i++){
			m_sceneLayers[i]->update();
		}
	}

	void Scene::render(){
		{
			GLenum error = glGetError();
			if(error != GL_NO_ERROR){
				VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
				DebugBreak();
			}

		}

		glBindTextureUnit(m_frameTextureID, m_frameTexture.getTextureID());
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_frameTexture.getWidth(), m_frameTexture.getHeight());
		glClearColor(0.1, 0.13f, 0.2f, 1.0f);//

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for(int i = 0; i < m_sceneLayers.size(); i++){
			m_sceneLayers[i]->render();

		}
	}
	void Scene::onLoad(){
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

	void Scene::onEvent(){
		for(int i = 0; i < m_sceneLayers.size(); i++){
			m_sceneLayers[i]->onEvent();
		}
	}

	void Scene::onAttach(){
		m_frameTextureID = 13;
		VOFOG_CORE_LOG("Scene: {}\tLoading...", m_sceneName);
		/*if(m_sceneLoaded == false)
			initialize();*/
	}



}