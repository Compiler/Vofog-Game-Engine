#pragma once
#include <Vofog.h>
#include <glm/glm.hpp>

namespace Vofog{

	
	class VOFOG_API PickingRay{
	private:

		glm::vec3 clickPosInWorld;
		glm::vec3 direction;

	public:
		/**
		 * Computes the intersection of this ray with the X-Y Plane (where Z = 0)
		 * and writes it back to the provided vector.
		 */
		void intersectionWithXyPlane(glm::vec3 worldPos){
			float s = -clickPosInWorld.z / direction.z;
			worldPos.x = clickPosInWorld.x + direction.x * s;
			worldPos.y = clickPosInWorld.y + direction.y * s;
			worldPos.z = 0;
		}

		glm::vec3 getClickPosInWorld(){
			return clickPosInWorld;
		}
		glm::vec3 getDirection(){
			return direction;
		}
	}

}