#pragma once
#include <Vofog.h>
#include <string>
#include <glm/vec3.hpp>
namespace Vofog{
	namespace ModelLoader{
		struct VOFOG_API OBJModelData{
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> textureCoords;
			std::vector<glm::vec3> vertexNormals;

		};
	
		extern std::vector<float> loadModel(std::string path);

	};

}