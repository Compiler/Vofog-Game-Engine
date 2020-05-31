#include <vpch.h>
#include "Buffer.h"

namespace Vofog{

	Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount): _componentCount(componentCount){
		init(data, count, componentCount);
	}

	void Buffer::init(GLfloat* data, GLsizei count, GLuint componentCount){
		_componentCount = componentCount;
		glGenBuffers(1, &_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
	}

	void Buffer::bind() const{
		//VOFOG_CORE_TRACE("{}:\tBound array buffer.", _bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
	}

	void Buffer::unbind() const{
		//VOFOG_CORE_TRACE("{}:\tUnbound array buffer.", _bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Buffer::~Buffer(){
		glDeleteBuffers(1, &_bufferID);
	}
}
