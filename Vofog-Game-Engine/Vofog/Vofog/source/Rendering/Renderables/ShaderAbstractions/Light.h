#pragma once
#include <Vofog.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vofog{

	typedef std::string LightName;

	class VOFOG_API Light{


	protected:
		glm::vec3 m_position;
		glm::vec3 m_ambientStrength;
		glm::vec3 m_diffuseStrength;
		glm::vec3 m_specularStrength;
		glm::vec4 m_color;

		glm::mat4 m_lightProjection = glm::mat4(1.0f);
		glm::mat4 m_lightView = glm::mat4(1.0f);

		float m_reach, m_intensity;

		LightName m_name;
		std::string m_uniformName;

	public:

		Light(){}
		Light(glm::vec3 position, glm::vec3 ambientStrength, glm::vec3 diffuseStrength, glm::vec3 specularStrength, glm::vec4 color){
			init(position, ambientStrength, diffuseStrength, specularStrength, color);
		}
		Light(LightName name, glm::vec3 position, glm::vec3 ambientStrength, glm::vec3 diffuseStrength, glm::vec3 specularStrength, glm::vec4 color){
			init(position, ambientStrength, diffuseStrength, specularStrength, color);
			m_name = name;
		}
		void init(glm::vec3 position, glm::vec3 ambientStrength, glm::vec3 diffuseStrength, glm::vec3 specularStrength, glm::vec4 color){
			m_position = position; m_ambientStrength = ambientStrength; m_diffuseStrength = diffuseStrength;
			m_specularStrength = specularStrength; m_color = color;
		}
		void init(LightName name, glm::vec3 position, glm::vec3 ambientStrength, glm::vec3 diffuseStrength, glm::vec3 specularStrength, glm::vec4 color){
			m_position = position; m_ambientStrength = ambientStrength; m_diffuseStrength = diffuseStrength;
			m_specularStrength = specularStrength; m_color = color;
			m_name = name;
		}

		virtual void setReach(float reach){ m_reach = reach; }
		virtual void setIntensity(float intensity){ m_intensity = intensity; }

		virtual void setPosition(glm::vec3 position){ m_position = position; }
		virtual void setPosition(float x, float y, float z){ m_position.x = x; m_position.y = y; m_position.z = z; }

		virtual void setAmbientStrength(glm::vec3 ambientStrength){
			m_ambientStrength = ambientStrength;
		}
		virtual void setAmbientStrength(float x, float y, float z){
			m_ambientStrength.x = x;
			m_ambientStrength.y = y;
			m_ambientStrength.z = z;
		}
		virtual void setAmbientStrength(float strength){
			m_ambientStrength.x = strength;
			m_ambientStrength.y = strength;
			m_ambientStrength.z = strength;
		}

		virtual void setDiffuseStrength(glm::vec3 diffuseStrength){
			m_diffuseStrength = diffuseStrength;
		}
		virtual void setDiffuseStrength(float x, float y, float z){
			m_diffuseStrength.x = x;
			m_diffuseStrength.y = y;
			m_diffuseStrength.z = z;
		}
		virtual void setDiffuseStrength(float strength){
			m_diffuseStrength.x = strength;
			m_diffuseStrength.y = strength;
			m_diffuseStrength.z = strength;
		}

		virtual void setSpecularStrength(glm::vec3 specularStrength){
			m_specularStrength = specularStrength;
		}
		virtual void setSpecularStrength(float x, float y, float z){
			m_specularStrength.x = x;
			m_specularStrength.y = y;
			m_specularStrength.z = z;
		}
		virtual void setSpecularStrength(float strength){
			m_specularStrength.x = strength;
			m_specularStrength.y = strength;
			m_specularStrength.z = strength;
		}

		virtual void setColor(glm::vec4 color){
			m_color = color;
		}
		virtual void setSpecularStrength(float r, float g, float b, float a){
			m_color.x = r;
			m_color.y = g;
			m_color.z = b;
			m_color.w = a;
		}

		virtual void setLightProjection(glm::mat4 projection){
			m_lightProjection = projection;
		}
		
		virtual void generateLightView(glm::vec3 focusPoint = glm::vec3(0)){ m_lightView = glm::lookAt(m_position, focusPoint, glm::vec3(0, 1, 0)); }
		
		virtual glm::mat4 getProjection() const{ return m_lightProjection; }
		virtual glm::mat4 getView() const{ return m_lightView; }
		virtual glm::mat4 getCombined() const{ return m_lightProjection * m_lightView; }
		virtual void setLightName(LightName name){ m_name = name; }
		virtual void setUniformName(std::string name){ m_uniformName = name; }

		glm::vec3 getPosition() const{ return m_position; }
		glm::vec3 getAmbientStrength() const{ return m_ambientStrength; }
		glm::vec3 getDiffuseStrength() const{ return m_diffuseStrength; }
		glm::vec3 getSpecularStrength() const{ return m_specularStrength; }
		glm::vec4 getColor() const{ return m_color; }






		template<class Archive>
		void save(Archive& ar)const{
			ar(m_name, m_uniformName, m_position.x, m_diffuseStrength.x, m_color.x,
				m_position.y, m_diffuseStrength.y, m_color.y,
				m_position.z, m_diffuseStrength.z, m_color.z,
				m_ambientStrength.x, m_specularStrength.x, m_color.w,
				m_ambientStrength.y, m_specularStrength.y,
				m_ambientStrength.z, m_specularStrength.z,
				m_lightProjection[0].x, m_lightProjection[1].x, m_lightProjection[2].x, m_lightProjection[3].x,
				m_lightProjection[0].y, m_lightProjection[1].y, m_lightProjection[2].y, m_lightProjection[3].y,
				m_lightProjection[0].z, m_lightProjection[1].z, m_lightProjection[2].z, m_lightProjection[3].z,
				m_lightProjection[0].w, m_lightProjection[1].w, m_lightProjection[2].w, m_lightProjection[3].w,

				m_lightView[0].x, m_lightView[1].x, m_lightView[2].x, m_lightView[3].x,
				m_lightView[0].y, m_lightView[1].y, m_lightView[2].y, m_lightView[3].y,
				m_lightView[0].z, m_lightView[1].z, m_lightView[2].z, m_lightView[3].z,
				m_lightView[0].w, m_lightView[1].w, m_lightView[2].w, m_lightView[3].w, m_reach, m_intensity
				);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(m_name, m_uniformName, m_position.x, m_diffuseStrength.x, m_color.x,
				m_position.y, m_diffuseStrength.y, m_color.y,
				m_position.z, m_diffuseStrength.z, m_color.z,
				m_ambientStrength.x, m_specularStrength.x, m_color.w,
				m_ambientStrength.y, m_specularStrength.y,
				m_ambientStrength.z, m_specularStrength.z,
				m_lightProjection[0].x, m_lightProjection[1].x, m_lightProjection[2].x, m_lightProjection[3].x,
				m_lightProjection[0].y, m_lightProjection[1].y, m_lightProjection[2].y, m_lightProjection[3].y,
				m_lightProjection[0].z, m_lightProjection[1].z, m_lightProjection[2].z, m_lightProjection[3].z,
				m_lightProjection[0].w, m_lightProjection[1].w, m_lightProjection[2].w, m_lightProjection[3].w,

				m_lightView[0].x, m_lightView[1].x, m_lightView[2].x, m_lightView[3].x,
				m_lightView[0].y, m_lightView[1].y, m_lightView[2].y, m_lightView[3].y,
				m_lightView[0].z, m_lightView[1].z, m_lightView[2].z, m_lightView[3].z,
				m_lightView[0].w, m_lightView[1].w, m_lightView[2].w, m_lightView[3].w, m_reach, m_intensity
			);
		}



		virtual ~Light(){}
	};
}