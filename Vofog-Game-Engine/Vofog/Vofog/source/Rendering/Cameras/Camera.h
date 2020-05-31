#pragma once

#include <Vofog.h>
#include <glm/glm.hpp>
#include <glad/glad.h>


#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/polymorphic.hpp>


namespace Vofog{

	class VOFOG_API Camera{

	protected:

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_combined;

		std::string m_cameraName;

		bool m_needsUpdate;

	public:
		Camera(){}

		virtual void update() = 0;
		virtual void translate(glm::vec3 translation) = 0;
		virtual void lookAt(glm::vec3 translation) = 0;
		virtual void resize(GLfloat newWidth, GLfloat newHeight) = 0;
		virtual void setCameraPosition(glm::vec3 position) = 0;

		virtual void rotate(glm::vec3 axis, GLfloat amount){};
		virtual void rotateView(glm::vec3 axis, GLfloat amount){};
		virtual void zoom(GLfloat yVal, GLfloat xVal = 0){};
		virtual void move(glm::vec3 direction){}

		glm::mat4 getProjectionMatrix() const{ return m_projectionMatrix; };
		glm::mat4 getViewMatrix() const{ return m_viewMatrix; };
		glm::mat4 getCombinedMatrix() const{ return m_combined; }

		virtual glm::vec3 getDirection() const{ return glm::vec3(0); };

		void setCameraName(std::string name){ m_cameraName = name; }

		virtual glm::vec3 getPosition() const = 0;




		template<class Archive>
		void save(Archive& ar) const{
			ar(m_cameraName, 
				m_projectionMatrix[0].x,
				m_projectionMatrix[1].x,
				m_projectionMatrix[2].x,
				m_projectionMatrix[3].x,
				m_projectionMatrix[0].y,
				m_projectionMatrix[1].y,
				m_projectionMatrix[2].y,
				m_projectionMatrix[3].y,
				m_projectionMatrix[0].z,
				m_projectionMatrix[1].z,
				m_projectionMatrix[2].z,
				m_projectionMatrix[3].z,
				m_projectionMatrix[0].w,
				m_projectionMatrix[1].w,
				m_projectionMatrix[2].w,
				m_projectionMatrix[3].w, 

				 m_viewMatrix[0].x,
				 m_viewMatrix[1].x,
				 m_viewMatrix[2].x,
				 m_viewMatrix[3].x,
				 m_viewMatrix[0].y,
				 m_viewMatrix[1].y,
				 m_viewMatrix[2].y,
				 m_viewMatrix[3].y,
				 m_viewMatrix[0].z,
				 m_viewMatrix[1].z,
				 m_viewMatrix[2].z,
				 m_viewMatrix[3].z,
				 m_viewMatrix[0].w,
				 m_viewMatrix[1].w,
				 m_viewMatrix[2].w,
				 m_viewMatrix[3].w, 

				m_combined[0].x,
				m_combined[1].x,
				m_combined[2].x,
				m_combined[3].x,
				m_combined[0].y,
				m_combined[1].y,
				m_combined[2].y,
				m_combined[3].y,
				m_combined[0].z,
				m_combined[1].z,
				m_combined[2].z,
				m_combined[3].z,
				m_combined[0].w,
				m_combined[1].w,
				m_combined[2].w,
				m_combined[3].w);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(m_cameraName,
				m_projectionMatrix[0].x,
				m_projectionMatrix[1].x,
				m_projectionMatrix[2].x,
				m_projectionMatrix[3].x,
				m_projectionMatrix[0].y,
				m_projectionMatrix[1].y,
				m_projectionMatrix[2].y,
				m_projectionMatrix[3].y,
				m_projectionMatrix[0].z,
				m_projectionMatrix[1].z,
				m_projectionMatrix[2].z,
				m_projectionMatrix[3].z,
				m_projectionMatrix[0].w,
				m_projectionMatrix[1].w,
				m_projectionMatrix[2].w,
				m_projectionMatrix[3].w, 
				
				m_viewMatrix[0].x,
				m_viewMatrix[1].x,
				m_viewMatrix[2].x,
				m_viewMatrix[3].x,
				m_viewMatrix[0].y,
				m_viewMatrix[1].y,
				m_viewMatrix[2].y,
				m_viewMatrix[3].y,
				m_viewMatrix[0].z,
				m_viewMatrix[1].z,
				m_viewMatrix[2].z,
				m_viewMatrix[3].z,
				m_viewMatrix[0].w,
				m_viewMatrix[1].w,
				m_viewMatrix[2].w,
				m_viewMatrix[3].w, 
				
				m_combined[0].x,
				m_combined[1].x,
				m_combined[2].x,
				m_combined[3].x,
				m_combined[0].y,
				m_combined[1].y,
				m_combined[2].y,
				m_combined[3].y,
				m_combined[0].z,
				m_combined[1].z,
				m_combined[2].z,
				m_combined[3].z,
				m_combined[0].w,
				m_combined[1].w,
				m_combined[2].w,
				m_combined[3].w);
			m_needsUpdate = true;
		}


		//virtual bool operator==(const Camera& other) = 0;

	};


}

