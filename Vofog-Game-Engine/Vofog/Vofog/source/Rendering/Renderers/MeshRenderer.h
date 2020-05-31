#pragma once
#include <Vofog.h>
#include "BaseRenderer.h"

namespace Vofog{
	namespace Renderer{
	

		class VOFOG_API MeshRenderer : BaseRenderer{
		private:

			

			std::vector<int> _textureSlots;
			

			void _setBufferSlot(glm::vec3 pos, glm::vec4 col,
				glm::vec2 tex, glm::vec3 norm, glm::vec3 tangent, glm::vec3 biTangent, glm::vec4 textureIDs);
			

			float _populateTextureIDs(int textureID);

		public:
			MeshRenderer(){ init(); };

			void begin();
			void init();
			void submit_raw_model(std::vector<float> packedData, glm::vec3 posOffset);
			void submit(const std::shared_ptr<Vofog::ECS::EntityManager>& manager);
			void submit_mesh_test(const std::shared_ptr<Vofog::ECS::EntityManager>& manager);
			void submit_debug(const std::shared_ptr<Vofog::ECS::EntityManager>& manager);
			void flush();
			void end();





			


			~MeshRenderer();

		};
	
	
	}

}