#pragma once
#include <Vofog.h>
#include <Rendering/Renderables/Renderer2D.h>
#include <Rendering/Renderables/IndexBuffer.h>
#include <Rendering/Renderables/Cube.h>
#include <VSL/HashMap.h>

#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Entity.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/RenderableComponent.h>
#include <Entity Component System/Components/TransformComponent.h>


#include <unordered_map>

namespace Vofog{

	typedef GLushort vfg_indice_type;

	#define VOFOG_RENDERER_MAX_SPRITES 50000
	#define VOFOG_RENDERER_VERTEX_SIZE sizeof(Vofog::VertexData)
	#define VOFOG_RENDERER_CUBE_SIZE 4 * 6 * VOFOG_RENDERER_VERTEX_SIZE
	#define VOFOG_RENDERER_BUFFER_SIZE VOFOG_RENDERER_MAX_SPRITES * VOFOG_RENDERER_VERTEX_SIZE 
	#define VOFOG_RENDERER_MAX_INDICE_SIZE (VOFOG_RENDERER_MAX_SPRITES * 6) 

	#define SHADER_VERTEX_ATTRIB_LOCATION		0
	#define SHADER_COLOR_ATTRIB_LOCATION		1
	#define SHADER_TEXCOORD_ATTRIB_LOCATION		2
	#define SHADER_NORMAL_ATTRIB_LOCATION		3
	#define SHADER_TANGENT_ATTRIB_LOCATION		4
	#define SHADER_BI_TANGENT_ATTRIB_LOCATION	5
	#define SHADER_TEXTURE_ID_ATTRIB_LOCATION	6


	class ModelGlyph{

		public:
			GLuint textureID;
			std::vector<VertexData> vertexData;


	};

	class VOFOG_API BatchRenderer : public Vofog::Renderer2D{

	private:

		std::unique_ptr<Vofog::IndexBuffer> _indexBuffer;

		//unsigned int _indiceCount;
		Vofog::VertexData* _buffer;

		unsigned int _bufferID, _vertexArrayID;

		std::vector<std::shared_ptr<Vofog::Cube>> m_models3D;
		std::vector<int> _textureSlots;


		void _sortAndBatch();
		void _setBufferSlot(glm::vec3 pos, glm::vec4 col,
			glm::vec2 tex, glm::vec3 norm, glm::vec3 tangent, glm::vec3 biTangent, glm::vec4 textureIDs);
		float _populateTextureIDs(int textureToCheck);
	
	public:
		BatchRenderer();

		void begin();
		void init();
		void submit2(const std::shared_ptr<Vofog::ECS::EntityManager>& manager);
		void submit(std::shared_ptr<Vofog::Renderable> renderable) override{}
		void submit(std::shared_ptr<Vofog::Cube> cube);
		void flush() override;
		void end();





		inline const int getSpriteCount() const{ return m_renderableCount; }
		
		
		bool operator==(const BatchRenderer& other)const{
			return false;
		
		}
		
		
		~BatchRenderer();


		

		template<class Archive>
		void save(Archive& ar)const{
			ar(m_models3D);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(m_models3D);

			init();
		}
	};

}