#include "vpch.h"
#include "InternalTexture.h"



namespace Vofog{


	InternalTexture::InternalTexture(const char name[], GLfloat width, GLfloat height, GLint internalFormat, GLint format, unsigned int dataType, unsigned int wrapTypeS, unsigned int wrapTypeT){
		init(name, width, height, internalFormat, format, dataType, wrapTypeS, wrapTypeT);

	}

	void InternalTexture::init(const char name[], GLfloat width, GLfloat height, GLint internalFormat, GLint format, unsigned int dataType, unsigned int wrapTypeS, unsigned int wrapTypeT){
		_handle = name;
		_internalFormat = internalFormat;
		_format = format;
		_wrapTypeS = wrapTypeS;
		_wrapTypeT = wrapTypeT;
		_dataType = dataType;
		this->_textureDimensions = glm::vec2(width, height);
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypeT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, NULL);

		glGenerateMipmap(GL_TEXTURE_2D);
		
	}




}