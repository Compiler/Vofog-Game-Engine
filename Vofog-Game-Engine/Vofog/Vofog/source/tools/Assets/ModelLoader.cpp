#include "vpch.h"
#include "ModelLoader.h"

namespace Vofog{


	std::vector<float> ModelLoader::loadModel(std::string path){
		ModelLoader::OBJModelData data;
		std::vector<float> packedData;
		FILE* file = fopen(path.c_str(), "r");
		if(!file) VOFOG_CORE_ERROR("{}:\tCouldn't open up file: {}", __FILENAME__, path);
		while(true){
			char lineHeader[256];
			int res = fscanf(file, "%s", lineHeader);
			if(res == EOF) break;

			//first word of line is "v"
			if(strcmp(lineHeader, "v") == 0){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				data.vertices.push_back(vertex);
			//	VOFOG_CORE_LOG("Pushed back: ({}, {}, {}) == ({}, {}, {})", vertex.x, vertex.y, vertex.z, data.vertices.back().x, data.vertices.back().y, data.vertices.back().z);
			}

			else if(strcmp(lineHeader, "vt") == 0){
				glm::vec2 vertex;
				fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
				data.textureCoords.push_back(vertex);
			//	VOFOG_CORE_TRACE("Pushed back: ({}, {}) == ({}, {})", vertex.x, vertex.y, data.textureCoords.back().x, data.textureCoords.back().y);
			}
			else if(strcmp(lineHeader, "vn") == 0){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				data.vertexNormals.push_back(vertex);
			//	VOFOG_CORE_WARN("Pushed back: ({}, {}, {}) == ({}, {}, {})", vertex.x, vertex.y, vertex.z, data.vertices.back().x, data.vertices.back().y, data.vertices.back().z);
			}

			else if(strcmp(lineHeader, "f") == 0){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				vertexIndex[0]--;	uvIndex[0]--;	 normalIndex[0]--;
				vertexIndex[1]--;	uvIndex[1]--;	 normalIndex[1]--;
				vertexIndex[2]--;	uvIndex[2]--;	 normalIndex[2]--;
				if(matches != 9){
					VOFOG_CORE_ERROR("File can't be read by our simple parser:\tTry exporting with other options\n");
					break;
				}
				packedData.push_back(data.vertices[vertexIndex[0]].x); packedData.push_back(data.vertices[vertexIndex[0]].y); packedData.push_back(data.vertices[vertexIndex[0]].z);
				packedData.push_back(data.textureCoords[uvIndex[0]].x); packedData.push_back(data.textureCoords[uvIndex[0]].y); 
				packedData.push_back(data.vertexNormals[normalIndex[0]].x); packedData.push_back(data.vertexNormals[normalIndex[0]].y); packedData.push_back(data.vertexNormals[normalIndex[0]].z);
				
				packedData.push_back(data.vertices[vertexIndex[1]].x); packedData.push_back(data.vertices[vertexIndex[1]].y); packedData.push_back(data.vertices[vertexIndex[1]].z);
				packedData.push_back(data.textureCoords[uvIndex[1]].x); packedData.push_back(data.textureCoords[uvIndex[1]].y); 
				packedData.push_back(data.vertexNormals[normalIndex[1]].x); packedData.push_back(data.vertexNormals[normalIndex[1]].y); packedData.push_back(data.vertexNormals[normalIndex[1]].z);
				
				packedData.push_back(data.vertices[vertexIndex[2]].x); packedData.push_back(data.vertices[vertexIndex[2]].y); packedData.push_back(data.vertices[vertexIndex[2]].z);
				packedData.push_back(data.textureCoords[uvIndex[2]].x); packedData.push_back(data.textureCoords[uvIndex[2]].y); 
				packedData.push_back(data.vertexNormals[normalIndex[2]].x); packedData.push_back(data.vertexNormals[normalIndex[2]].y); packedData.push_back(data.vertexNormals[normalIndex[2]].z);
				
				//VOFOG_CORE_DEBUG("Pushed back packed data [vertices, texCoords, normals]");
			}
		}


		return packedData;
	}

}