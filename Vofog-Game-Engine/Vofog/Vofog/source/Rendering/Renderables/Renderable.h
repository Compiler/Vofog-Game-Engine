#pragma once

#include <Vofog.h>
#include <glm/glm.hpp>
#include <Rendering/ShaderProgram.h>
#include <Rendering/Renderables/Buffer.h>
#include <Rendering/Renderables/Texture.h>
#include <Rendering/Renderables/VertexArray.h>
#include <Rendering/Renderables/Material.h>
#include "glm/gtc/matrix_transform.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>

namespace Vofog{


	struct VertexData{

		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoords;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 biTangent;
		glm::vec4 textureIDs;

	};


			
	class VOFOG_API Renderable{
	
	private:
		bool _submitted = false;;

	protected:
		glm::vec3 m_position;
		glm::vec3 m_size;
		glm::vec4 m_color;
		glm::mat4 m_model = glm::mat4(1.0f);
		std::string m_guiName;
		bool m_hasChanged = true;

		Vofog::Material m_material;


	public:
		Renderable(){}
		Renderable(glm::vec3 position, glm::vec3 size, const glm::vec4& tint = glm::vec4(1, 1, 1, 1))
			:m_position(position), m_size(size), m_color(tint){
			CREATION_COUNT++;
			m_guiName = "Renderable ";
			m_guiName += std::to_string(CREATION_COUNT);
		}

		void init(glm::vec3 position, glm::vec3 size, const glm::vec4& tint = glm::vec4(1, 1, 1, 1)){
			m_position = position; m_size = size; m_color = tint;
			CREATION_COUNT++;
			m_guiName = "Renderable ";
			m_guiName += std::to_string(CREATION_COUNT);
		}

		void setTexture(std::shared_ptr<Vofog::Texture> texture){
			m_material.setDiffuseTexture(texture);
		}

		void setMaterial(Vofog::Material mat){
			m_material = mat;
		}

		void emplaceMaterial(Texture diffuse, Texture specular, float shininess){
			m_material.init(diffuse, specular, shininess);
		}

		virtual Vofog::Material getMaterial(){ return m_material; }

		virtual ~Renderable(){
		}

		void setGuiName(const char name[]){
			m_guiName = name;
		}

		const char* getGuiName(){
			return m_guiName.c_str();
		}
		std::string getName(){
			return m_guiName;
		}

		bool submitted(){ return _submitted; }
		void setSubmitted(){ _submitted = true; }

		void setPosition(glm::vec3 pos){ if(m_position != pos) m_hasChanged = true; m_position = pos;}
		void setX(GLfloat x){ if(m_position.x != x) m_hasChanged = true; m_position.x = x;}
		void setY(GLfloat y){ if(m_position.y != y) m_hasChanged = true; m_position.y = y; }
		void setZ(GLfloat z){ if(m_position.z != z) m_hasChanged = true; m_position.z = z; }

		void setSize(glm::vec2 size){
			if(m_size.x != size.x || m_size.y != size.y) m_hasChanged = true;
			m_size.x = size.x; m_size.y = size.y;
		}
		void setSize(glm::vec3 size){ if(m_size != size) m_hasChanged = true; m_size = size; }
		void setWidth(GLfloat width){ if(m_size.x != width) m_hasChanged = true; m_size.x = width; }
		void setHeight(GLfloat height){ if(m_size.y != height) m_hasChanged = true; m_size.y = height; }
		void setDepth(GLfloat depth){ if(m_size.z != depth) m_hasChanged = true; m_size.z = depth; }

		void setHasChanged(bool changed){ m_hasChanged = changed; }

		inline const glm::vec3 getSize() const{return m_size;}
		inline const glm::vec3 getPosition() const{return m_position;}
		inline const glm::vec4 getColor() const{return m_color;}
		inline const glm::mat4& getModelMatrix() const{ return m_model; }
		inline const bool hasChanged() const { return m_hasChanged; }
		
		std::shared_ptr<Texture> getTexture(){ return m_material.getDiffuseTexture(); }
		int getTextureID(){ return m_material.getDiffuseTextureID(); }
		void translateModel(glm::vec3 translation){
			//m_model = glm::translate(m_model, translation);
			m_position += translation;
			m_hasChanged = true;
		}

		void rotateModel(glm::vec3 axis, float amount){
			glm::vec3 centroi = glm::vec3(m_position.x + m_size.x / 2.0f,
				m_position.y + m_size.y / 2.0f, m_position.z - m_size.z / 2.0f);
			m_model = glm::translate(m_model, centroi);
			m_model = glm::rotate(m_model, glm::radians(amount), axis);
			m_model = glm::translate(m_model, -centroi);

			m_hasChanged = true;
		}


		template<class Archive>
		void save(Archive& ar) const {
			ar(m_guiName, m_material,
				m_position.x, m_position.y, m_position.z,
				m_size.x, m_size.y, m_size.z,
				m_color.x, m_color.y, m_color.z, m_color.w,
				m_model[0].x, m_model[1].x, m_model[2].x, m_model[3].x,
				m_model[0].y, m_model[1].y, m_model[2].y, m_model[3].y,
				m_model[0].z, m_model[1].z, m_model[2].z, m_model[3].z,
				m_model[0].w, m_model[1].w, m_model[2].w, m_model[3].w, true);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(m_guiName, m_material,
				m_position.x, m_position.y, m_position.z,
				m_size.x, m_size.y, m_size.z,
				m_color.x, m_color.y, m_color.z, m_color.w,
				m_model[0].x, m_model[1].x, m_model[2].x, m_model[3].x, 
				m_model[0].y, m_model[1].y, m_model[2].y, m_model[3].y, 
				m_model[0].z, m_model[1].z, m_model[2].z, m_model[3].z, 
				m_model[0].w, m_model[1].w, m_model[2].w, m_model[3].w, m_hasChanged);
		}


		static unsigned int CREATION_COUNT;

	};
	
}