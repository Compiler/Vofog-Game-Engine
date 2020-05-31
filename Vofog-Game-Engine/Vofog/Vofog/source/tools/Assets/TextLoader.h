#pragma once
#include <tools/Logging.h>
#include <glad/glad.h>
#include <istream>
#include <string>
#include <sstream>
#include <fstream>


//#pragma warning (disable : 4996)
namespace Vofog {



	/*Name of file with extension, not the file path. All file paths go to resources/shaders/fileName */
	static GLchar* loadShader(const char fileName[]){

		std::string filePath = "../Vofog/resources/shaders/";
		filePath += fileName;
		
		
		std::ifstream in(filePath, std::ios::in);
		
		if(!in.is_open()){
			VOFOG_CORE_ERROR("Couldn't open up: '{}'", filePath);
			_VOFOG_DEBUG_BREAK_;
		}
		//else VOFOG_CORE_DEBUG("File: '{}' {}", filePath, "successfully opened.");

		std::string streamCode;
		try{
			in.open(filePath);
			std::stringstream vertexShaderStream;
			vertexShaderStream << in.rdbuf();
			in.close();
			streamCode = vertexShaderStream.str();
		} catch (std::ifstream::failure e) {
			VOFOG_CORE_ERROR("File couldn't be read");
			return nullptr;
		}
		std::string key = "VOFOG_TEXTURE_SLOTS ";
		int start = streamCode.find(key) ;
		if (start != std::string::npos) {
			start += key.length();
			int end = streamCode.find("\n", start);
			GLint textureUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
			streamCode.replace(start, end - start, std::to_string(textureUnits));
		}
		GLchar* vertexShaderCode = new GLchar[streamCode.size() + 1];
		memcpy(vertexShaderCode, streamCode.c_str(), streamCode.size() + 1);

		return vertexShaderCode;

	}

}