#pragma once
#include <Vofog.h>
#include <glad/glad.h>


namespace Vofog{

	class VOFOG_API IndexBuffer{

	private:
		GLuint _indexID, _indiceCount;

	public:
		IndexBuffer(){}
		IndexBuffer(GLuint* indices, GLuint indiceCount);
		IndexBuffer(GLushort* indices, GLuint indiceCount);
		void init(GLuint* indices, GLuint indiceCount);
		void init(GLushort* indices, GLuint indiceCount);

		void bind()const;
		void unbind()const;

		inline GLuint getIndiceCount() const{ return _indiceCount; }

		~IndexBuffer();
	};

}