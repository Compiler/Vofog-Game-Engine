#pragma once


#include <Vofog.h>

#include <tools/Assets/TextLoader.h>
#include <glad/glad.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog {



	enum SHADER_TYPE{
		VERTEX, FRAGMENT, GEOMETRY

	};

	enum UniformTypes{

		FLOAT = 0, INT = 1, BOOL = 2, VEC2F = 3, VEC3F = 4,
		VEC4F = 5, MAT2F = 6, MAT3F = 7, MAT4F = 8, SAMPLER2D = 9,
		
		ITEM_COUNT = 10
	};



	struct UniformData{
		std::vector<std::string> floatNames;
		std::unordered_map<std::string, float> floats;

		

		unsigned int uniformTypeCount[UniformTypes::ITEM_COUNT] = {0} ;

		UniformData(){
			
		}

		UniformData(const UniformData& newVersion){
			this->floatNames = newVersion.floatNames;
			this->floats = newVersion.floats;
		}
	};

	typedef std::string GLSLName;

	struct GLSLCustomDS{
		std::string name;
		UniformData data;

		GLSLCustomDS(){}
		GLSLCustomDS(const std::string& dataName){
			name = dataName;
		}
		GLSLCustomDS(const std::string& dataName, int index){
			name = dataName + "[" + std::to_string(index) + "]";
		}
		GLSLCustomDS(const GLSLCustomDS& newVersion){
			this->name = newVersion.name;
			this->data = newVersion.data;
		}
		GLSLCustomDS& operator=(const GLSLCustomDS&) = default;

	};
#define VOFOG_GLSL_DS_KEY "VFG_"
#define VOFOG_GLSL_DS_KEY_LENGTH 4
	class VOFOG_API ShaderProgram{

	private:


		UniformData _generalUniformData;//All uniforms that dont belong to a GLSL struct

		std::unordered_map<GLSLName, std::vector<GLSLCustomDS>> _customGLSLDataStructures;//Maps a name to a list of custom GLSL data structures
		std::vector<GLSLName> _customDSNames;//names of custom GLSL data structures
		void _updateCustomDataVec1(const char name[], float data);

		/*Shader program ID*/
		GLuint _programID;

		/*ID for shaders*/
		GLuint _vertexID, _fragmentID, _geometryID;
		bool useGeo = false;
		
		/*Keeps the code clean. Helper functions for seperating shader compilation and linkage into stages*/
		void _compileShaders();
		void _linkShaders();


		void _parseCodeForCustomDataStructures(std::string file, const char* type);

		std::string _shaderName;
		std::string _vertexPath, _fragmentPath, _geometryPath;
		char _shaderCharName[128];

		/*Grabs uniform with given name and throws warning if it couldn't be found*/
		#define _VOFOG_UNIFORM_CHECK(x) \
		GLint uniformID = glGetUniformLocation(_programID, x);\
		if(uniformID == -1)\
			VOFOG_CORE_WARN("'{}' name not found in shader: {}", x, _shaderName);\
		glUseProgram(_programID)


		void _parseUniforms();

		const char* _getGLName(const char name[], unsigned int enumVal);

	public:
		/*Default does nothing, call init if using this.*/
		ShaderProgram();

		/*File NAME for path, will search through resources/shaders/ for fileName*/
		ShaderProgram(const char vertexFilePath[], const char fragmentFilePath[], std::string geometryFilePath = "");


		/*File NAME for path, will search through resources/shaders/ for fileName*/
		void init(const char vertexFilePath[], const char fragmentFilePath[], std::string geometryFilePath = "");


		void use() const { glUseProgram(_programID); }

		/*Sets uniform with uniform name to the array of size count*/
		void setUniform1iv(const char* uniformName, int count, GLint* f1);
		void setUniform1fv(const char* uniformName, int count, GLfloat* f1);

		/*Sets uniform with uniformName equal to the x ints given*/
		void setUniformInt1(const char* uniformName, GLint f1);

		/*Sets uniform with uniformName equal to the x floats given*/
		void setUniformVec1(const char* uniformName, GLfloat f1);

		void setUniformVec2(const char* uniformName, GLfloat f1, GLfloat f2);
		void setUniformVec3(const char* uniformName, GLfloat f1, GLfloat f2, GLfloat f3);
		void setUniformVec4(const char* uniformName, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

		void setUniformVec2(const char* uniformName, glm::vec2 values);
		void setUniformVec3(const char* uniformName, glm::vec3 values);
		void setUniformVec4(const char* uniformName, glm::vec4 values);



		/*Set uniform with uniformName to matrix given*/
		void setUniformMat4(const char* uniformName, const GLfloat *value);



		UniformData* getUniformData(){ return &_generalUniformData; }
		std::vector<std::string>* getFloatNames(){ return &(_generalUniformData.floatNames); }
		const char* getShaderName()const{ return _shaderName.c_str(); }
		GLuint getShaderProgramID() const{ return _programID; }


		const std::vector<GLSLName>* getCustomDSNames()const{ return &(_customDSNames); }
		const std::vector<GLSLCustomDS>* getCustomGLSLDSList(GLSLName key){ return &_customGLSLDataStructures[key]; }
		
		
		std::string getVertexPath()const{ return _vertexPath; }
		std::string getFragmentPath()const{ return _fragmentPath; }
		std::string getGeometryPath()const{ return _geometryPath; }
		
		
		template<class Archive>
		void save(Archive& ar)const{
			ar(_shaderName, _vertexPath, _fragmentPath, _geometryPath);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(_shaderName, _vertexPath, _fragmentPath, _geometryPath);
			init(_vertexPath.c_str(), _fragmentPath.c_str(), _geometryPath);
		}

		
		
		~ShaderProgram();

	};
}
