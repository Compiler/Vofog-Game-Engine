#pragma once
#include <glad/glad.h>
#include <Rendering/Renderables/Buffer.h>

namespace Vofog{

	class VOFOG_API VertexArray{

	private:
		GLuint _vertexArrayID;
		std::vector<Vofog::Buffer*> _buffers;

	public:
		VertexArray();
		void addBuffer(Vofog::Buffer* buffer, GLuint vertexAttribIndex);
		void init();
		void bind() const;
		void unbind() const;


		~VertexArray();
	};

}