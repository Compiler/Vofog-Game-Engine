#pragma once
#include <Vofog.h>
#include <tools/Assets/ImageLoader.h>
#include <glad/glad.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog{
	enum class TextureFormat {
		RGB = 0,
		RGBA = 1,
		S_RGB = 2,
		S_RGBA = 3

	};
	class VOFOG_API Texture{

	private:
		
		GLuint _textureID = 0;
		std::string _filePath;

		glm::vec2 _textureDimensions;
		int _internalFormat, _format;
		unsigned int _wrapTypeS, _wrapTypeT, _dataType;
		std::string _handle;


		
	public:
		Texture(){
			_filePath = "";
			_handle = "nothing";
		}
		Texture(const char name[], GLint internalFormat, GLint format, unsigned int dataType = GL_UNSIGNED_BYTE,
			unsigned int wrapTypeS = GL_REPEAT, unsigned int wrapTypeT = GL_REPEAT);

		void init(const char name[], GLint internalFormat, GLint format,
			unsigned int dataType = GL_UNSIGNED_BYTE, unsigned int wrapTypeS = GL_REPEAT, unsigned int wrapTypeT = GL_REPEAT);

		
		void bind();
		void activate();

		void unbind();

		float getWidth()const{ return _textureDimensions.x; }
		float getHeight()const{ return _textureDimensions.y; }
		std::string getFilePath()const{ return _filePath; }
		std::string getHandle()const{ return _handle; }
		GLuint getTextureID()const{ return _textureID; }
			
		void setHandle(std::string newHandle){ _handle = newHandle; }
		
		template<class Archive>
		void save(Archive& ar)const{
			ar(_filePath, _handle, _internalFormat, _format, _wrapTypeS, _wrapTypeT, _dataType);
			VOFOG_CORE_LOG("Saved {} to {}", _handle, _filePath);
		}

		template<class Archive>
		void load(Archive& ar){
			std::string currentHandle = _handle;
			ar(_filePath, _handle, _internalFormat, _format, _wrapTypeS, _wrapTypeT, _dataType);
			VOFOG_CORE_LOG("Loading {} to {}", _handle, _filePath);

			init(_filePath.c_str(), _internalFormat, _format, _dataType, _wrapTypeS, _wrapTypeT);
			_handle = currentHandle;

		}
		
		

		~Texture();
	};

}
