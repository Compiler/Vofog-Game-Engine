#pragma once

#include <Vofog.h>
#include <Rendering/Renderables/Renderable.h>
#include <Rendering/Renderables/Sprite2D.h>

namespace Vofog{

	class VOFOG_API Cube : public Renderable{

	private:
		VertexArray _vertexArray;

	public:
		Cube(){}
		Cube(glm::vec3 position, glm::vec3 size, glm::vec4 color = glm::vec4(1,1,1,1)):
			Renderable(glm::vec3(position.x, position.y, position.z), glm::vec3(size.x, size.y, size.z), color){
		}
		void init(glm::vec3 position, glm::vec3 size, glm::vec4 color = glm::vec4(1, 1, 1, 1)){
			Renderable(glm::vec3(position.x, position.y, position.z), glm::vec3(size.x, size.y, size.z), color);
		}
		


		template<class Archive>
		void save(Archive& ar) const{
			ar(cereal::base_class<Renderable>(this));
		}

		template<class Archive>
		void load(Archive& ar){
			ar(cereal::base_class<Renderable>(this));
		}
		


		


	};


}