#pragma once
#include <Vofog.h>
#include <Rendering/Renderables/Texture.h>
#include <Rendering/ShaderProgram.h>

#include <Rendering/TextureHandler.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog{
	class VOFOG_API Material{
	#define VOFOG_MISSING_TEXTURE -4444
	private:
		std::shared_ptr<Vofog::Texture> _diffuse;
		std::shared_ptr<Vofog::Texture> _specular;
		std::shared_ptr<Vofog::Texture> _normal;
		std::shared_ptr<Vofog::Texture> _height;
		float _shininess;

	public:
		Material();
		Material(Vofog::Texture diffuse, Vofog::Texture specular, float shininess);
		Material(const Material& old){
			_diffuse = old.getDiffuseTexture();
			_specular = old.getSpecularTexture();
			_normal = old.getNormalTexture();
			_height = old.getHeightTexture();
			_shininess = old.getShininess();
		};
		Material& operator=(const Material& old){
			_diffuse = old.getDiffuseTexture();
			_specular = old.getSpecularTexture();
			_normal = old.getNormalTexture();
			_height = old.getHeightTexture();
			_shininess = old.getShininess();
			return *this;
		}
		void init(Vofog::Texture diffuse, Vofog::Texture specular, float shininess);
		void init(std::shared_ptr<Vofog::Texture> diffuse, std::shared_ptr<Vofog::Texture> specular, float shininess);
		void applyToShader(ShaderProgram& program);

		void setDiffuseTexture(std::shared_ptr<Vofog::Texture> diffuse);
		void setSpecularTexture(std::shared_ptr<Vofog::Texture> specular);
		void setNormalTexture(std::shared_ptr<Vofog::Texture> normal);
		void setHeightTexture(std::shared_ptr<Vofog::Texture> height);
		void setShininess(float shininess);



		int hashFunction() const;

		bool operator==(const Material& other) const;

		std::shared_ptr<Vofog::Texture> getDiffuseTexture()const{ return _diffuse; }
		std::shared_ptr<Vofog::Texture> getSpecularTexture()const{ return _specular; }
		std::shared_ptr<Vofog::Texture> getNormalTexture()const{ return _normal; }
		std::shared_ptr<Vofog::Texture> getHeightTexture()const{ return _height; }


		int getDiffuseTextureID()const { if (_diffuse != nullptr) return _diffuse->getTextureID(); return VOFOG_MISSING_TEXTURE; }
		int getSpecularTextureID()const { if (_specular != nullptr) return _specular->getTextureID(); return VOFOG_MISSING_TEXTURE; }
		int getNormalTextureID()const { if (_normal != nullptr) return _normal->getTextureID(); return VOFOG_MISSING_TEXTURE; }
		int getHeightTextureID()const { if (_height != nullptr) return _height->getTextureID(); return VOFOG_MISSING_TEXTURE; }
		float getShininess() const{ return _shininess; }


		std::string getDebugInfo(){
			return _diffuse->getFilePath() + ", " + _specular->getFilePath();
		}

		
		template<class Archive>
		void save(Archive& ar)const{
			VOFOG_CORE_LOG("Diffuse Texture\nHandle: '{}' \tFilePath: '{}' ", _diffuse->getHandle(), _diffuse->getFilePath());
			VOFOG_CORE_LOG("Specular Texture\nHandle: '{}' \tFilePath: '{}' ", _specular->getHandle(), _specular->getFilePath());
			VOFOG_CORE_LOG("Normal Texture\nHandle: '{}' \tFilePath: '{}' ", _normal->getHandle(), _normal->getFilePath());
			VOFOG_CORE_LOG("HeightMap Texture\nHandle: '{}' \tFilePath: '{}' ", _height->getHandle(), _height->getFilePath());
			ar(_diffuse->getHandle(), _specular->getHandle(), _normal->getHandle(), _height->getHandle(), _shininess);
			//ar(_diffuse, _specular, _normal, _height, _shininess);
		}

		template<class Archive>
		void load(Archive& ar){
			std::string diffuseHandle, specularHandle, normalHandle, heightHandle;
			//ar(_diffuse, _specular, _normal, _height, _shininess);



			ar(diffuseHandle, specularHandle, normalHandle, heightHandle, _shininess);
			_diffuse = TextureHandler::get(diffuseHandle);
			_specular = TextureHandler::get(specularHandle);
			_normal = TextureHandler::get(normalHandle);
			_height = TextureHandler::get(heightHandle);
			//init(_diffuse, _specular, _shininess);
		}


		~Material();
	};


}