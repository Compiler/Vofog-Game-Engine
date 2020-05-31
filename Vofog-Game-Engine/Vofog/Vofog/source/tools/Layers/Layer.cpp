#include <vpch.h>
#include "Layer.h"


namespace Vofog{


	Layer::Layer(){
		m_layerName = "DefaultAbstractLayer" + std::to_string(rand());
	}

	Layer::Layer(std::shared_ptr<ECS::EntityManager> entityManager){
		m_layerName = "DefaultAbstractLayer" + std::to_string(rand());
		m_entityManager = entityManager;
	}

	Layer::Layer(GLfloat viewportWidth, GLfloat viewportHeight){
		init(viewportWidth, viewportHeight);
		m_layerName = "DefaultAbstractLayer" + std::to_string(rand());
	}

	void Layer::init(float viewportWidth, float viewportHeight){
		m_entityManager = std::make_shared<ECS::EntityManager>();

		m_meshRenderer = std::make_shared<Vofog::Renderer::MeshRenderer>(); m_meshRenderer->init();
		m_camera = std::make_shared<Vofog::PerspectiveCamera>(45, Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT);
		m_shaderProgram = std::make_shared<ShaderProgram>("pass.vert", "pass.frag", "");
		m_shaderProgram->use();
		GLint textureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
		for(int i = 0; i < textureUnits; i++){
			m_shaderProgram->setUniformInt1(std::string(std::string("u_textures[") + std::to_string(i) + "]").c_str(), i);
		}

	}


	void Layer::update(){
		m_entityManager->updateSystems();
	}

	void Layer::onLoad(){
		TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "cheapbricks_diffuse.jpg", "SRGB bricks", GL_SRGB, GL_RGB);//srgb
		TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "container.png", "SRGBA container", GL_SRGB_ALPHA, GL_RGBA);//srgba
		m_meshRenderer = std::make_shared<Renderer::MeshRenderer>(); m_meshRenderer->init();

	}

}