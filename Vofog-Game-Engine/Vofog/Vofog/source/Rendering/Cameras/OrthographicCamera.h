#pragma once
#include <Vofog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Cameras/Camera.h>


namespace Vofog{
	class VOFOG_API OrthographicCamera : public Camera{

	private:
		GLfloat _left, _right, _top, _bottom, _nearZ, _farZ, _aspectRatio;
		glm::vec3 _position;
	public:
		OrthographicCamera(){}
		OrthographicCamera(GLfloat width, GLfloat height, GLfloat nearZ = 0.1f, GLfloat farZ = 100.0f);
		OrthographicCamera(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ = 0.1f, GLfloat farZ = 100.0f);

		void update() override;
		void resize(GLfloat newWidth, GLfloat newHeight)override;
		void translate(glm::vec3 translation) override;
		void lookAt(glm::vec3 translation) override;
		void setCameraPosition(glm::vec3 position)override;

		template<class Archive>
		void save(Archive& ar)const{
			ar(cereal::base_class<Camera>(this),
				_left, _right, _top, _bottom,
				_nearZ, _farZ, _aspectRatio);
		}

		template<class Archive>
		void load(Archive& ar){
			ar(cereal::base_class<Camera>(this),
				_left, _right, _top, _bottom,
				_nearZ, _farZ, _aspectRatio);
		}
		
		glm::vec3 getPosition() const override{ return glm::vec3(_left, _bottom, 0); };

		bool operator==(const OrthographicCamera& other){
			if(m_projectionMatrix == other.m_projectionMatrix && m_cameraName == other.m_cameraName && m_viewMatrix == other.m_viewMatrix &&
				other.m_combined == m_combined && _left == other._left && _right == other._right && _top == other._top && _nearZ == other._nearZ &&
				_farZ == other._farZ && _bottom == other._bottom && _aspectRatio == other._aspectRatio) return true;
			else return false;

		}



	};
}