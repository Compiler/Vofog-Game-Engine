#include <vpch.h>
#include "VertexArray.h"


namespace Vofog{

	VertexArray::VertexArray(){

		//glGenVertexArrays(1, &_vertexArrayID);
		

	}

	void VertexArray::init(){
		glGenVertexArrays(1, &_vertexArrayID);
	}

	void VertexArray::addBuffer(Vofog::Buffer* buffer, GLuint vertexAttribIndex){
		_buffers.emplace_back(buffer);

		bind();
		buffer->bind();

		//glEnableVertexAttribArray(_vertexArrayID);
		glEnableVertexAttribArray(vertexAttribIndex);
		glVertexAttribPointer(vertexAttribIndex, buffer->getComponentCount(), 
			GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

		//unbind();
		//buffer->unbind();

	}

	void VertexArray::bind() const{
		//VOFOG_CORE_TRACE("{}:\tBound VertexArray.", _vertexArrayID);
		glBindVertexArray(_vertexArrayID);
	}

	void VertexArray::unbind() const{
		//VOFOG_CORE_TRACE("{}:\tUnbound VertexArray.", _vertexArrayID);
		glBindVertexArray(0);
	}


	VertexArray::~VertexArray(){
		for(int i = 0; i < _buffers.size(); i++){
			delete _buffers[i];
		}

		glDeleteVertexArrays(1, &_vertexArrayID);

	}

}