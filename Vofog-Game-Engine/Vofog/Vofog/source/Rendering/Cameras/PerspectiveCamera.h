#pragma once

#include <Vofog.h>
#include <Rendering/Cameras/Camera.h>
#include <Rendering/Cameras/PerspectiveCamera.h>
#include <glm/common.hpp>

namespace Vofog{

	class VOFOG_API PerspectiveCamera : public Camera{

	private:
		float _fov, _width, _height, _nearZ, _farZ;
		glm::vec3 _zeroVec = glm::vec3(0, 0, 0);

		glm::vec3 _cameraPosition, _cameraDirection, _cameraFront, _cameraUp, _cameraRight;
					//-90			   0	
		float _yaw = -90, _pitch = 0, _zoom = 0;

	public:
		PerspectiveCamera();
		PerspectiveCamera(float fov, float width, float height, float nearZ = 0.1f, float farZ = 1500.0f);



		void update() override;
		void resize(float newWidth, float newHeight)override;
		void translate(glm::vec3 translation) override;
		void lookAt(glm::vec3 translation) override;
		void rotate(glm::vec3 axis, float amount) override;
		void rotateView(glm::vec3 axis, float amount)override;
		void zoom(float yVal, float xVal = 0)override;

		void setCameraPosition(glm::vec3 position)override;

		void setPitch(GLfloat amount){};

		void move(glm::vec3 direction);



		glm::vec3 getDirection() const override{ return _cameraFront; };
		glm::vec3 getPosition() const override{ return _cameraPosition; };

		
		template<class Archive>
		void save(Archive& ar)const{
			ar(cereal::base_class<Camera>(this),
				_fov, _width, _height,
				_nearZ, _farZ,
				_cameraPosition.x, _cameraPosition.y, _cameraPosition.z,
				_cameraDirection.x, _cameraDirection.y, _cameraDirection.z,
				_cameraFront.x, _cameraFront.y, _cameraFront.z,
				_cameraUp.x, _cameraUp.y, _cameraUp.z,
				_cameraRight.x, _cameraRight.y, _cameraRight.z, _yaw, _pitch);


		}

		template<class Archive>
		void load(Archive& ar){
			ar(cereal::base_class<Camera>(this),
				_fov, _width, _height,
				_nearZ, _farZ,
				_cameraPosition.x, _cameraPosition.y, _cameraPosition.z,
				_cameraDirection.x, _cameraDirection.y, _cameraDirection.z,
				_cameraFront.x, _cameraFront.y, _cameraFront.z,
				_cameraUp.x, _cameraUp.y, _cameraUp.z,
				_cameraRight.x, _cameraRight.y, _cameraRight.z, _yaw, _pitch);


		}


		bool operator==(const PerspectiveCamera& other){
			if(m_projectionMatrix == other.m_projectionMatrix && m_cameraName == other.m_cameraName && m_viewMatrix == other.m_viewMatrix &&
				other.m_combined == m_combined && _fov == other._fov && _width == other._width && _height == other._height && _nearZ == other._nearZ &&
				_farZ == other._farZ) return true;
			else return false;
		
		}



		~PerspectiveCamera();
	};

}
CEREAL_REGISTER_TYPE(Vofog::PerspectiveCamera);
