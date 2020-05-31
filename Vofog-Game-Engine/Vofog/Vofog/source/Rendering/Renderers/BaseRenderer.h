#pragma once
#include <Vofog.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Rendering/Renderables/IndexBuffer.h>


#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Entity.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/RenderableComponent.h>
#include <Entity Component System/Components/TransformComponent.h>

namespace Vofog{

	namespace Renderer{
		struct VertexData{
			glm::vec3 position;
			glm::vec4 color;
			glm::vec2 texCoords;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec3 biTangent;
			glm::vec4 textureIDs;
		};

		typedef GLushort vofog_indice_type;

		#define VOFOG_NULL_TEXTURE 0

		#define VOFOG_RENDERER_MAX_SPRITES 150000
		#define VOFOG_RENDERER_VERTEX_SIZE sizeof(Vofog::Renderer::VertexData)
		#define VOFOG_RENDERER_BUFFER_SIZE VOFOG_RENDERER_MAX_SPRITES * VOFOG_RENDERER_VERTEX_SIZE 
		#define VOFOG_RENDERER_MAX_INDICE_SIZE (VOFOG_RENDERER_MAX_SPRITES * 3) 

		#define SHADER_VERTEX_ATTRIB_LOCATION		0
		#define SHADER_COLOR_ATTRIB_LOCATION		1
		#define SHADER_TEXCOORD_ATTRIB_LOCATION		2
		#define SHADER_NORMAL_ATTRIB_LOCATION		3
		#define SHADER_TANGENT_ATTRIB_LOCATION		4
		#define SHADER_BI_TANGENT_ATTRIB_LOCATION	5
		#define SHADER_TEXTURE_ID_ATTRIB_LOCATION	6

		class VOFOG_API BaseRenderer{
		protected:
			std::unique_ptr<Vofog::IndexBuffer> m_indexBuffer;
			std::vector<int> m_textureSlots;
			int m_currentIndex = 0;
			unsigned int m_indiceCount = 0;

			Vofog::Renderer::VertexData* m_buffer;
			vofog_indice_type* m_dynamicIndexBuffer;

			unsigned int m_bufferID, m_vertexArrayID, m_indexID;



			/*virtual void init() = 0;
			virtual void begin();
			virtual void submit(const std::shared_ptr<Vofog::ECS::EntityManager>& manager) = 0;
			virtual void end() = 0;
			virtual void flush() = 0;*/
		};


	}

}