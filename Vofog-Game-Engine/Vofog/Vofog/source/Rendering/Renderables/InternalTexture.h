#pragma once

#include <Vofog.h>

#include <glad/glad.h>


#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog{


	class VOFOG_API InternalTexture{
		private:

		GLuint _textureID = 0;
		
		glm::vec2 _textureDimensions;
		int _internalFormat, _format;
		unsigned int _dataType, _wrapTypeS, _wrapTypeT;
		std::string _handle;



		public:
		InternalTexture(){
			_handle = "nothing";
		}
		InternalTexture(const char name[], GLfloat width, GLfloat height, GLint internalFormat, GLint format, unsigned int dataType = GL_UNSIGNED_BYTE,
			unsigned int wrapTypeS = GL_REPEAT, unsigned int wrapTypeT = GL_REPEAT);

		void init(const char name[], GLfloat width, GLfloat height, GLint internalFormat, GLint format,
			unsigned int dataType = GL_UNSIGNED_BYTE, unsigned int wrapTypeS = GL_REPEAT, unsigned int wrapTypeT = GL_REPEAT);



		float getWidth()const{ return _textureDimensions.x; }
		float getHeight()const{ return _textureDimensions.y; }
		std::string getHandle()const{ return _handle; }
		GLuint getTextureID()const{ return _textureID; }

		void setHandle(std::string newHandle){ _handle = newHandle; }

		template<class Archive>
		void save(Archive& ar)const{
			//VOFOG_CORE_DEBUG("Save: Internal Texture : {} w/ Dimensions: ({}, {})", _handle, _textureDimensions.x, _textureDimensions.y);
			ar(_handle, _textureDimensions.x, _textureDimensions.y, _internalFormat, _format, _dataType, _wrapTypeS, _wrapTypeT);

		}

		template<class Archive>
		void load(Archive& ar){
			ar(_handle, _textureDimensions.x, _textureDimensions.y, _internalFormat, _format, _dataType, _wrapTypeS, _wrapTypeT);
		//	VOFOG_CORE_DEBUG("Load: Internal Texture : {} w/ Dimensions: ({}, {})", _handle, _textureDimensions.x, _textureDimensions.y);
			init(_handle.c_str(), _textureDimensions.x, _textureDimensions.y, _internalFormat, _format, _dataType, _wrapTypeS, _wrapTypeT);
	

		}
	};



}
