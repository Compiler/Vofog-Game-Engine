#include "vpch.h"
#include "IndexBuffer.h"



namespace Vofog{
	IndexBuffer::IndexBuffer(GLuint* indices, GLuint indiceCount): _indiceCount(indiceCount){
		glGenBuffers(1, &_indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	}
	IndexBuffer::IndexBuffer(GLushort* indices, GLuint indiceCount) : _indiceCount(indiceCount){
		glGenBuffers(1, &_indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
	}

	void IndexBuffer::init(GLuint* indices, GLuint indiceCount){
		_indiceCount = indiceCount;
		glGenBuffers(1, &_indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}
	void IndexBuffer::init(GLushort* indices, GLuint indiceCount){
		_indiceCount = indiceCount;
		glGenBuffers(1, &_indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
	}

	void IndexBuffer::bind() const{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexID); }
	void IndexBuffer::unbind() const{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }



	IndexBuffer::~IndexBuffer(){
		glDeleteBuffers(1, &_indexID);
	}
	
}