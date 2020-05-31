#include "vpch.h"
#include "Material.h"

namespace Vofog{

	Material::Material(){
		_diffuse = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_specular = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_normal = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_height = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
	}
	
	Material::Material(Vofog::Texture diffuse, Vofog::Texture specular, float shininess):
		_diffuse(std::make_shared<Vofog::Texture>(diffuse)), _specular(std::make_shared<Vofog::Texture>(specular)), _shininess(shininess){
		_normal = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_height = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
	}

	void Material::init(Vofog::Texture diffuse, Vofog::Texture specular, float shininess){
		_diffuse = std::make_shared<Vofog::Texture>(diffuse); _specular = std::make_shared<Vofog::Texture>(specular); _shininess = shininess;
		_normal = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_height = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
	}

	void Material::init(std::shared_ptr<Vofog::Texture> diffuse, std::shared_ptr<Vofog::Texture> specular, float shininess){
		_diffuse = diffuse; _specular = specular; _shininess = shininess;
		if(_diffuse == nullptr) _diffuse = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		else if (_diffuse->getFilePath().empty())_diffuse = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		if(_specular == nullptr) _specular = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		else if(_specular->getFilePath().empty())_specular = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_normal = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
		_height = TextureHandler::get(VOFOG_DEFAULT_IMAGE_FILEPATH + "Default.png");
	}

	void Material::applyToShader(ShaderProgram& program){
		program.use();
		_diffuse->bind();
		program.setUniformInt1("u_material.diffuse", _diffuse->getTextureID());
		_specular->bind();
		program.setUniformInt1("u_material.specular", _specular->getTextureID());
		program.setUniformVec1("u_material.shininess", _shininess);
	
	}



	int Material::hashFunction() const{
		return (_diffuse->getTextureID() * 1000) + _specular->getTextureID();
	}
	
	bool Material::operator==(const Material& other) const{
		return (this->getDiffuseTexture()->getTextureID() == other.getDiffuseTexture()->getTextureID()) &&
			(this->getSpecularTexture()->getTextureID() == other.getSpecularTexture()->getTextureID());
	}




	void Material::setDiffuseTexture(std::shared_ptr<Vofog::Texture> diffuse){ _diffuse = diffuse; }
	void Material::setSpecularTexture(std::shared_ptr<Vofog::Texture> specular){ _specular = specular; }
	void Material::setNormalTexture(std::shared_ptr<Vofog::Texture> normal) { _normal = normal; }
	void Material::setHeightTexture(std::shared_ptr<Vofog::Texture> height) { _height = height; }
	void Material::setShininess(float shininess){ _shininess = shininess; }

	Material::~Material(){
	}


}