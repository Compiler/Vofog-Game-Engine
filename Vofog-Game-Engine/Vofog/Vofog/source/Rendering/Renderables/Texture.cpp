#include "vpch.h"
#include "Texture.h"
namespace Vofog{

	Texture::Texture(const char name[], GLint internalFormat, GLint format, unsigned int dataType, unsigned int wrapTypeS, unsigned int wrapTypeT){
		init(name, internalFormat, format, dataType, wrapTypeS, wrapTypeT);

	}

	void Texture::init(const char name[], GLint internalFormat, GLint format, unsigned int dataType, unsigned int wrapTypeS, unsigned int wrapTypeT){
		_filePath = name;
		_handle = _filePath;
		_internalFormat = internalFormat;
		_format = format;
		_wrapTypeS = wrapTypeS;
		_wrapTypeT = wrapTypeT;
		_dataType = dataType;
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypeT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		std::string filePath = /*std::string("../Vofog/resources/images/")*/ +name;
		unsigned char* data = Vofog::ImageLoader::loadImage(filePath.c_str(), &width, &height, &nrChannels);
		if(data){
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, data);
			this->_textureDimensions = glm::vec2(width, height);
			glGenerateMipmap(GL_TEXTURE_2D);
			Vofog::ImageLoader::free(data);
			VOFOG_CORE_LOG("Succeeded to load texture {}", name);

		} else{
			VOFOG_CORE_FATAL("Failed to load texture {}", name);
			_VOFOG_DEBUG_BREAK_;
		}
	}
	/* else{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, nullWidth, nullHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
		this->_textureDimensions = glm::vec2(nullWidth, nullHeight);

		glGenerateMipmap(GL_TEXTURE_2D);*/

		//bind();
	void Texture::bind(){
		activate();
		glBindTexture(GL_TEXTURE_2D, _textureID);
		VOFOG_CORE_LOG("Bound {} to slot {}", _filePath, _textureID);
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
	}
	void Texture::activate(){
		glActiveTexture(GL_TEXTURE0 + _textureID);
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
	}

	void Texture::unbind(){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);


	}


	Texture::~Texture(){
	}

}