#include "vpch.h"
#include "PerspectiveCamera.h"
#include <glm\gtc\matrix_transform.hpp>


namespace Vofog{


	PerspectiveCamera::PerspectiveCamera(){}
	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearZ, float farZ)
		:_fov(fov), _width(width), _height(height), _nearZ(nearZ), _farZ(farZ){
	
		m_projectionMatrix = glm::perspective(glm::radians(fov), width / height, nearZ, farZ);
		_cameraPosition = glm::vec3(0, 0, 3);
		_cameraFront = glm::vec3(0, 0, -1);	
		_cameraUp = glm::vec3(0, 1, 0);
		m_viewMatrix = glm::lookAt(_cameraPosition, _cameraPosition + _cameraFront, _cameraUp);


		m_combined = m_projectionMatrix * m_viewMatrix;
		//_update();
		m_needsUpdate = true;
		glm::vec3 front;
		front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		front.y = sin(glm::radians(_pitch));
		front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_cameraFront = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		_cameraRight = glm::normalize(glm::cross(_cameraFront, glm::vec3(0, 1, 0)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		_cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
	}

	void PerspectiveCamera::update(){
		if(m_needsUpdate){
			glm::vec3 front;
			front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			front.y = sin(glm::radians(_pitch));
			front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			_cameraFront = glm::normalize(front);
			// Also re-calculate the Right and Up vector
			_cameraRight = glm::normalize(glm::cross(_cameraFront, glm::vec3(0, 1, 0)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			_cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
			
			//_cameraFront += _zoom / 100.0f;
			m_viewMatrix = glm::lookAt(_cameraPosition, _cameraPosition + _cameraFront, _cameraUp);
			m_combined = m_projectionMatrix * m_viewMatrix;
			m_needsUpdate = false;
		}

	}
	void PerspectiveCamera::resize(float newWidth, float newHeight){
		if(newWidth != _width || newHeight != _height){
			_width = newWidth; _height = newHeight;
			m_projectionMatrix = glm::perspective(glm::radians(_fov), _width / _height, _nearZ, _farZ);
			m_needsUpdate = true;
		}

	}
	void PerspectiveCamera::translate(glm::vec3 translation){
		if(translation != _zeroVec){
			_cameraPosition += _cameraRight * translation;
			_cameraPosition += _cameraUp * translation;
			_cameraPosition -= _cameraFront * translation;


			//m_viewMatrix = glm::lookAt(_cameraPosition, glm::vec3(0.f, 0.f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			//m_viewMatrix = glm::translate(m_viewMatrix, translation);
			m_needsUpdate = true;
		}
	}
	
	void PerspectiveCamera::lookAt(glm::vec3 translation){
	
	}

	void PerspectiveCamera::setCameraPosition(glm::vec3 position){
		VOFOG_CORE_LOG("Translating from ({},{},{}) to ({},{},{}) using translation ({},{},{})",
			_cameraPosition.x, _cameraPosition.y, _cameraPosition.z, position.x, position.y, position.z,
			(position - _cameraPosition).x, (position - _cameraPosition).y, (position - _cameraPosition).z);
		translate(-_cameraPosition);
		update();
		translate(position);
		VOFOG_CORE_LOG("New camera position: ({}, {}, {})", _cameraPosition.x, _cameraPosition.y, _cameraPosition.z);
		//m_viewMatrix = glm::lookAt(_cameraPosition, glm::vec3(0.f, 0.f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_needsUpdate = true;
	}

	void PerspectiveCamera::rotate(glm::vec3 axis, float amount){
		_pitch += axis.y * amount;
		//_yaw += axis.x * amount;
		_yaw = glm::mod(_yaw + (axis.x * amount), 360.0f);
		//_yaw = 

		if(_pitch > 89.0f)
			_pitch = 89.0f;
		if(_pitch < -89.0f)
			_pitch = -89.0f;

		if((axis.x != 0 || axis.y != 0) && amount != 0)
			m_needsUpdate = true;
		
	}

	void PerspectiveCamera::rotateView(glm::vec3 axis, float amount){
		setCameraPosition(glm::vec3(0, 0, 0));
		m_needsUpdate = true;
		update();
		rotate(axis, amount);

	}


	void PerspectiveCamera::zoom(float yVal, float xVal){
		VOFOG_CORE_DEBUG("Zoom value: {}", yVal);
		/*if(_fov >= 1.0f && _fov <= 45.0f)
			_fov -= yVal;
		if(_fov <= 1.0f)
			_fov = 1.0f;
		if(_fov >= 45.0f)
			_fov = 45.0f;

		//m_projectionMatrix = glm::perspective(glm::radians(_fov), _width / _height, _nearZ, _farZ);
		m_needsUpdate = true;*/

	}

	void PerspectiveCamera::move(glm::vec3 direction){
		float speed = 4.44f;
		if(direction.x > 0)
			_cameraPosition += _cameraRight * speed * ImGui::GetIO().DeltaTime;
		else if(direction.x < 0)
			_cameraPosition -= _cameraRight * speed * ImGui::GetIO().DeltaTime;

		if(direction.y > 0){
			_cameraPosition += _cameraFront * speed * ImGui::GetIO().DeltaTime;
			//_cameraPosition -= _cameraRight * speed;
		}
		else if(direction.y < 0)
			_cameraPosition -= _cameraFront * speed * ImGui::GetIO().DeltaTime;

		if(!(direction.x == 0 && direction.y == 0 && direction.z == 0))
			m_needsUpdate = true;
	}





	PerspectiveCamera::~PerspectiveCamera(){
	}

}