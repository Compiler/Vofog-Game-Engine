#pragma once
#include <Vofog.h>
#include <glad/glad.h>


namespace Vofog{

	class VOFOG_API Buffer{


	private:
		GLuint _bufferID, _componentCount;


	public:

		Buffer(){}

		Buffer(GLfloat* data, GLsizei count, GLuint componentCount);

		void init(GLfloat* data, GLsizei count, GLuint componentCount);

		GLuint getComponentCount() const{ return _componentCount; }

		void bind() const;
		void unbind()const;

		~Buffer();
		
		};

}