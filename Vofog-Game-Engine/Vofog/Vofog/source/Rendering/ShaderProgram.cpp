#include <vpch.h>
#include "ShaderProgram.h"

namespace Vofog {

	ShaderProgram::ShaderProgram(){
		//_shaderName = "";
	}


	ShaderProgram::ShaderProgram(const char vertexFilePath[], const char fragmentFilePath[], std::string geometryFilePath){
		init(vertexFilePath, fragmentFilePath, geometryFilePath);
	}


	void ShaderProgram::init(const char vertexFilePath[], const char fragmentFilePath[], std::string geometryFilePath) {
		_shaderName = vertexFilePath;
		_vertexPath = vertexFilePath;
		_fragmentPath = fragmentFilePath;

		//_shaderName[strlen(_shaderName) - 4] = '\0';
		if(!geometryFilePath.empty()){
			useGeo = true;
			_geometryPath = geometryFilePath;
		}
		/*Grab Vertex+Fragment shader source code for compilation*/
		_vertexID = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vertexSource;
		vertexSource = Vofog::loadShader(vertexFilePath);

		_parseCodeForCustomDataStructures(vertexSource, "Vertex");
		glShaderSource(_vertexID, 1, &vertexSource, NULL);

		_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragmentSource;
		fragmentSource = Vofog::loadShader(fragmentFilePath);



		_parseCodeForCustomDataStructures(fragmentSource, "Fragment");

		glShaderSource(_fragmentID, 1, &fragmentSource, NULL);
		
		if(useGeo == true){
			_geometryID = glCreateShader(GL_GEOMETRY_SHADER);
			const GLchar* geometrySource = Vofog::loadShader(geometryFilePath.c_str());
			_parseCodeForCustomDataStructures(geometrySource, "Geometry");

			glShaderSource(_geometryID, 1, &geometrySource, NULL);
			if(useGeo)delete(geometrySource);
		}

#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
#endif

		_compileShaders();
		_linkShaders();


		/*Free source code data and delete shaders*/
		delete(vertexSource);  delete(fragmentSource);
		glDeleteShader(_vertexID); glDeleteShader(_fragmentID);
		if(useGeo) glDeleteShader(_geometryID);
		
#ifdef VOFOG_DEBUG_BUILD
		error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

#endif

		_parseUniforms();

	}


	void ShaderProgram::_parseUniforms(){
		GLint params;
		auto a = 6;
		glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &params);
		const short BUF_SIZE = 256;
		VOFOG_CORE_DEBUG("Uniforms of {} shader.", _shaderName);

		
		for(int i = 0; i < params; i++){
			GLchar name[BUF_SIZE];
			GLsizei uniformNameLength;
			GLint sizeOfUniform;
			GLenum uniformType;
			glGetActiveUniform(_programID, i, BUF_SIZE,
				&uniformNameLength, &sizeOfUniform, &uniformType, name);
			_getGLName(name, uniformType);
			//VOFOG_CORE_TRACE("{} \"{}\" : {} instances", _getGLName(name, uniformType), name, 1);
		}

		std::vector<int> eraseIndices;
		//take floats and put them into the custom DS
		for(int i = 0; i < _generalUniformData.floatNames.size(); i++){
			for(int k = 0; k < _customDSNames.size(); k++){
				std::string parsedName = _generalUniformData.floatNames.at(i);//full name ex: "u_VFG_PointLight[0].intensity"
				std::string uniformGLSLName = _customDSNames.at(k);//Simplified struct name: "VFG_PointLight"
				//	if(uniformGLSLName == "") continue;
				std::size_t nameLocation = parsedName.find(uniformGLSLName);
				//VOFOG_CORE_DEBUG("Searching for \"{}\" inside of \"{}\"", uniformGLSLName, parsedName);
				if(nameLocation != std::string::npos){
					//confirmed this float is part of a customDS, therefore delete from general purpose ds
					eraseIndices.push_back(i);
					//this is either an open bracket or a . i.e: PointLight[x].var or PointLight.var
					int separatorLocation = nameLocation + uniformGLSLName.size();
					if(parsedName.at(separatorLocation) == '['){
						//this grabs the index  x in something like : PointLight[x].var
						size_t indexOfGLSLArray = std::stoi(parsedName.substr(separatorLocation + 1, 1));
						//VOFOG_CORE_LOG("\tFound: \"{}\", \"{}\"", parsedName, uniformGLSLName);
						
						int varNameLocation = parsedName.find(".") + 1;//variable after the ., i.e PointLight[0].reach
						std::string variableName = parsedName.substr(varNameLocation, parsedName.size() - varNameLocation );
					//	VOFOG_CORE_LOG("\t\tVariable name: \"{}\"", variableName);
						std::vector<GLSLCustomDS>* mappedData = &_customGLSLDataStructures[uniformGLSLName];
						if(mappedData->at(indexOfGLSLArray).name == "")
							mappedData->at(indexOfGLSLArray).name = uniformGLSLName + "[" + std::to_string(indexOfGLSLArray) + "]";
						mappedData->at(indexOfGLSLArray).data.floatNames.push_back(variableName);
						mappedData->at(indexOfGLSLArray).data.floats[variableName] = 0;//default val
						//_customGLSLDataStructures.at(k)[indexOfGLSLArray].data.floatNames.push_back(variableName);
						//_customGLSLDataStructures.at(k)[indexOfGLSLArray].data.floats[variableName] = _generalUniformData.floats[_generalUniformData.floatNames.at(i)];
					}
				}
			}
		}
		
		//deletes customDS from general purpose uniformData
		for(int i = 0; i < eraseIndices.size(); i++){
			_generalUniformData.floats.erase(_generalUniformData.floatNames[eraseIndices[i]]);
			_generalUniformData.floatNames.erase(_generalUniformData.floatNames.begin() + eraseIndices[i]);
			for(int k = i; k < eraseIndices.size(); k++)
				eraseIndices[k]--;
		}

		//error is below


		for(int i = 0; i < _customDSNames.size(); i++){
			std::vector<GLSLCustomDS> mappedData = _customGLSLDataStructures[_customDSNames[i]];
			for(int k = 0; k <mappedData.size(); k++){
				//VOFOG_CORE_DEBUG("Name: \"{}\"", _customDSNames[i]);
				for(int x = 0; x < mappedData[k].data.floatNames.size(); x++){
					std::string thing = mappedData[k].data.floatNames.at(x);
					float data = mappedData[k].data.floats.at(thing);
					//VOFOG_CORE_LOG("\t{} = {}", thing, data);
				}
			}
		}

		//if(_generalUniformData.uniformTypeCount[UniformTypes::FLOAT] != 0) 
			//_generalUniformData.floatNames = std::make_unique<std::string[]>(_generalUniformData.uniformTypeCount[UniformTypes::FLOAT]);

		auto __debugHelper = [](unsigned int val){
			if(val == 0) return "float";
			if(val == 1) return "int";
			if(val == 2) return "bool";
			if(val == 3) return "vec2f";
			if(val == 4) return "vec3f";
			if(val == 5) return "vec4f";
			if(val == 6) return "mat2f";
			if(val == 8) return "mat3f";
			if(val == 7) return "mat4f";
			if(val == 9) return "Sampler2D";

			return "No Type found";
		};


		//VOFOG_CORE_DEBUG("Uniform data type count");
		//for(int i = 0; i < UniformTypes::ITEM_COUNT; i++)
			//VOFOG_CORE_TRACE("{}: {}", __debugHelper(i), _generalUniformData.uniformTypeCount[i]);


		VOFOG_CORE_ERROR("End");
	}



	/*Compile shaders and error check*/
	void ShaderProgram::_compileShaders(){
		
		
		glCompileShader(_vertexID);


								 /*Check for errors linking*/
		{//=============================Scoped for stack======================================
			int success;
			glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &success);
			if(success == GL_FALSE){
				int lengthOfError;
				glGetShaderiv(_vertexID, GL_INFO_LOG_LENGTH, &lengthOfError);
				char* infoLog = (char*)alloca(lengthOfError * sizeof(char));
				glGetShaderInfoLog(_vertexID, lengthOfError, &lengthOfError, infoLog);
				VOFOG_CORE_ERROR("Vertex shader compilation failed:\n{}", infoLog);
				glDeleteShader(_vertexID);
			} //else
				//VOFOG_CORE_DEBUG("Vertex shader compilation completed");
		}//===================================================================================


		glCompileShader(_fragmentID);


								 /*Check for errors linking*/
		{//=============================Scoped for stack======================================
			int success;
			glGetShaderiv(_fragmentID, GL_COMPILE_STATUS, &success);
			if(success == GL_FALSE){
				int lengthOfError;
				glGetShaderiv(_fragmentID, GL_INFO_LOG_LENGTH, &lengthOfError);
				char* infoLog = (char*)alloca(lengthOfError * sizeof(char));
				glGetShaderInfoLog(_fragmentID, lengthOfError, &lengthOfError, infoLog);
				VOFOG_CORE_ERROR("Fragment shader compilation failed:\n{}", infoLog);
				glDeleteShader(_fragmentID);
			} //else
				//VOFOG_CORE_DEBUG("Fragment shader compilation completed");
		}//===================================================================================


		if(useGeo){
			glCompileShader(_geometryID);


			/*Check for errors linking*/
			{//=============================Scoped for stack======================================
				int success;
				glGetShaderiv(_fragmentID, GL_COMPILE_STATUS, &success);
				if(success == GL_FALSE){
					int lengthOfError;
					glGetShaderiv(_fragmentID, GL_INFO_LOG_LENGTH, &lengthOfError);
					char* infoLog = (char*)alloca(lengthOfError * sizeof(char));
					glGetShaderInfoLog(_fragmentID, lengthOfError, &lengthOfError, infoLog);
					VOFOG_CORE_ERROR("Geometry shader compilation failed:\n{}", infoLog);
					glDeleteShader(_fragmentID);
				} //else
					//VOFOG_CORE_DEBUG("Geometry shader compilation completed");
			}//===================================================================================
		
		}
	}


	/*Link shaders and error check each step*/
	void ShaderProgram::_linkShaders(){

		_programID = glCreateProgram();
		for(int i = 0; i < _shaderName.size(); i++)
			_shaderCharName[i] = _shaderName.c_str()[i];
		//glObjectLabel(GL_SHADER, _programID, _shaderName.size(), _shaderName.c_str());
	#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
	#endif

		glAttachShader(_programID, _vertexID);
		glAttachShader(_programID, _fragmentID);
		if(useGeo)
			glAttachShader(_programID, _geometryID);
	#ifdef VOFOG_DEBUG_BUILD
		error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
	#endif
		glLinkProgram(_programID);

								 /*Check for errors linking*/
		{//=============================Scoped for stack======================================
			int success;
			glGetProgramiv(_programID, GL_LINK_STATUS, &success);
			if(!success){
				int lengthOfError;
				glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &lengthOfError);
				char* infoLog = (char*)alloca(lengthOfError * sizeof(char));
				glGetProgramInfoLog(_programID, lengthOfError, &lengthOfError, infoLog);
				VOFOG_CORE_ERROR("{}: Error linking shaders:\n{}", _shaderName, infoLog);
				assert(false);
			} 
		}//==================================================================================
	#ifdef VOFOG_DEBUG_BUILD
		error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
	#endif

	}

	void ShaderProgram::_updateCustomDataVec1(const char* uniformName, GLfloat f1){
		std::string stringifiedName = uniformName;
		int nameStartLocation = stringifiedName.find(VOFOG_GLSL_DS_KEY) + VOFOG_GLSL_DS_KEY_LENGTH;
		VOFOG_CORE_LOG("position: \"{}\"", stringifiedName.substr(nameStartLocation));

		size_t bracketLocation = stringifiedName.find('[');
		if(bracketLocation != std::string::npos){
			GLSLName glslDSName = stringifiedName.substr(2, bracketLocation-2);
			std::string arrayIndex = stringifiedName.substr(bracketLocation+1, stringifiedName.find(']') - bracketLocation-1);
			std::string memberName = stringifiedName.substr(stringifiedName.find('.') + 1);
			_customGLSLDataStructures[glslDSName].at(std::stoi(arrayIndex)).data.floats[memberName] = f1;
		}
	}
	void ShaderProgram::setUniformVec1(const char* uniformName, GLfloat f1){
		if(std::string(uniformName).find(VOFOG_GLSL_DS_KEY) != std::string::npos)
			_updateCustomDataVec1(uniformName, f1);
		else{
			if(_generalUniformData.floats.find(uniformName) == _generalUniformData.floats.end()){
				_generalUniformData.floatNames.emplace_back(uniformName);
				VOFOG_CORE_WARN("{}: Mapped {} to {}", _shaderName, uniformName, f1);
			}
			_generalUniformData.floats[uniformName] = f1;
		}
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro

		glUniform1f(uniformID, f1);
		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
		#endif

		/*for(int i = 0; i < _customDSNames.size(); i++){
			std::vector<GLSLCustomDS> mappedData = _customGLSLDataStructures[_customDSNames[i]];
			for(int k = 0; k < mappedData.size(); k++){
				VOFOG_CORE_DEBUG("Name: \"{}\"", _customDSNames[i]);
				for(int x = 0; x < mappedData[k].data.floatNames.size(); x++){
					std::string thing = mappedData[k].data.floatNames.at(x);
					float data = mappedData[k].data.floats.at(thing);
					VOFOG_CORE_LOG("\t{} = {}", thing, data);
				}
			}
		}*/
	}


	void ShaderProgram::setUniformVec2(const char* uniformName, GLfloat f1, GLfloat f2){
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform2f(uniformID, f1, f2);
		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}
	void ShaderProgram::setUniformVec2(const char* uniformName, glm::vec2 values){
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform2f(uniformID, values.x, values.y);

		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}

	void ShaderProgram::setUniformVec3(const char* uniformName, GLfloat f1, GLfloat f2, GLfloat f3){
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform3f(uniformID, f1, f2, f3);

		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}
	void ShaderProgram::setUniformVec3(const char* uniformName, glm::vec3 values){
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform3f(uniformID, values.x, values.y, values.z);

		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}
	void ShaderProgram::setUniformVec4(const char* uniformName, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4){

		
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform4f(uniformID, f1, f2, f3, f4);

		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}

	void ShaderProgram::setUniformVec4(const char* uniformName, glm::vec4 values){


		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform4f(uniformID, values.x, values.y, values.z, values.w);
		{
			GLenum error = glGetError();
			if(error != GL_NO_ERROR){
				VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
				DebugBreak();
			}
		}
		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);

		#endif
	}


	void ShaderProgram::setUniformMat4(const char* uniformName, const GLfloat *value){
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
		#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			VOFOG_CORE_ERROR("{}:{}\tOpenGL Error {}", __FILENAME__, __LINE__, error);
		#endif
	}


	void ShaderProgram::setUniformInt1(const char* uniformName, GLint f1) {
		
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform1i(uniformID, f1);

	#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_FATAL("{}:{}\tOpenGL Error {}\t{} couldnt be assigned {}", __FILENAME__, __LINE__, error, uniformName, f1);
	#endif
	}

	void ShaderProgram::setUniform1iv(const char* uniformName, int count, GLint* f1) {
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform1iv(uniformID, count, f1);

	#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_FATAL("{}:{}\tOpenGL Error {}\t{} couldnt be assigned {}", __FILENAME__, __LINE__, error, uniformName, f1[0]);
	#endif
	}

	void ShaderProgram::setUniform1fv(const char* uniformName, int count, GLfloat* f1) {
		_VOFOG_UNIFORM_CHECK(uniformName); //uniform ID was grabbed in the macro
		glUniform1fv(uniformID, count, f1);

	#ifdef VOFOG_DEBUG_BUILD
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			VOFOG_CORE_FATAL("{}:{}\tOpenGL Error {}\t{} couldnt be assigned {}", __FILENAME__, __LINE__, error, uniformName, f1[0]);
	#endif
	}


	const char* ShaderProgram::_getGLName(const char name[], unsigned int enumVal){
		if(enumVal == GL_FLOAT){
			_generalUniformData.uniformTypeCount[UniformTypes::FLOAT]++;
			_generalUniformData.floats[name] = 0;
			_generalUniformData.floatNames.emplace_back(name);
			return "float";
		}
		else if(enumVal == GL_SAMPLER_2D){
			_generalUniformData.uniformTypeCount[UniformTypes::SAMPLER2D]++;
			return "Sampler2D";
		}
		else if(enumVal == GL_BOOL){
			_generalUniformData.uniformTypeCount[UniformTypes::BOOL]++;
			return "bool";
		}
		else if(enumVal == GL_INT){
			_generalUniformData.uniformTypeCount[UniformTypes::INT]++;
			return "int";
		}
		else if(enumVal == GL_FLOAT_VEC2){
			_generalUniformData.uniformTypeCount[UniformTypes::VEC2F]++;
			return "vec2f";
		}
		else if(enumVal == GL_FLOAT_VEC3){
			_generalUniformData.uniformTypeCount[UniformTypes::VEC3F]++;
			return "vec3f";
		}
		else if(enumVal == GL_FLOAT_VEC4){
			_generalUniformData.uniformTypeCount[UniformTypes::VEC4F]++;
			return "vec4f";
		}
		else if(enumVal == GL_FLOAT_MAT2){
			_generalUniformData.uniformTypeCount[UniformTypes::MAT2F]++;
			return "mat2f";
		}
		else if(enumVal == GL_FLOAT_MAT3){
			_generalUniformData.uniformTypeCount[UniformTypes::MAT3F]++;
			return "mat3f";
		}
		else if(enumVal == GL_FLOAT_MAT4){
			_generalUniformData.uniformTypeCount[UniformTypes::MAT4F]++;
			return "mat4f";
		}
		return "ERROR_VAL";
	}


	void ShaderProgram::_parseCodeForCustomDataStructures(std::string file, const char* type){
		static std::string pre("struct ");
		int keyIndex = 0;
		keyIndex = file.find(pre + VOFOG_GLSL_DS_KEY);
		VOFOG_CORE_DEBUG("File {}, stage {}", _shaderName, type);
		while(keyIndex != std::string::npos){
			int indexOfEnd = file.find("{", keyIndex);
			std::string customDS = file.substr(keyIndex + pre.size(), indexOfEnd - (keyIndex + pre.size() + 1));
			VOFOG_CORE_LOG("Potential custom DS: {}", customDS);
			keyIndex = file.find(pre + VOFOG_GLSL_DS_KEY, keyIndex+1);

			int uniformDeclaration = file.find("[", file.find("uniform " + customDS));
			int uniformEndDeclaration = file.find("]", uniformDeclaration);
			std::string arrayCount = file.substr(uniformDeclaration + 1, uniformEndDeclaration - uniformDeclaration - 1);

			VOFOG_CORE_DEBUG("Array of {} contains {} elements", customDS, arrayCount);

			char start = arrayCount.at(0);
			//if starting value is NOT a number i.e [COUNT_...]
			int count = 0;
			while(!(start <= 57 && start >= 48)){
				std::string first = arrayCount;
				std::string searchString = "#define " + arrayCount;
				int macroStart = file.find(searchString);
				int macroEnd = file.find("\n", macroStart);
				arrayCount = file.substr(macroStart + searchString.size() + 1, macroEnd - (macroStart + searchString.size() + 1));
				start = arrayCount.at(0);
				VOFOG_CORE_LOG("Converting \"{}\" to \"{}\"", first, arrayCount);




				//TODO: Fix this v
				/*make sure in your glsl code that you only do concrete numbers or macros as the decider of your array size
				 you can do 
					#define buffer 200
					#define VFG_POINT_LIGHT_COUNT buffer

				but if you do 
					#define buffer 200
					#define VFG_POINT_LIGHT_COUNT buffer+13

				this will crash
				*/

				assert(count < 100);
				count++;
			}
			size_t arrCount = std::stoi(arrayCount);
			_customGLSLDataStructures[customDS] = std::vector<GLSLCustomDS>(arrCount);
			_customDSNames.push_back(customDS);
			//for(int i = 0; i < arrCount; i++)
				//_customGLSLDataStructures.back().at(i) = (GLSLCustomDS(customDS));
					
		}

	}

	
	ShaderProgram::~ShaderProgram(){
	}

}