#include <vpch.h>
#include "OrthographicCamera.h"

namespace Vofog{

	OrthographicCamera::OrthographicCamera(GLfloat width, GLfloat height, GLfloat nearZ, GLfloat farZ){
		_left = _bottom = 0; this->_nearZ = nearZ; this->_farZ = farZ;
		_top = height; _right = width;
		this->_aspectRatio = width / height;
		_position = glm::vec3(0, 0, 2);
		m_projectionMatrix = glm::ortho(0.0f, width, 0.0f, height, nearZ, farZ);

		m_viewMatrix = glm::lookAt(_position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));


		m_combined = m_projectionMatrix * m_viewMatrix;
	}
	OrthographicCamera::OrthographicCamera(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ){
		_left = _bottom = 0; this->_nearZ = nearZ; this->_farZ = farZ;
		_top = top; _right = right;
		this->_aspectRatio = (right - left) / (top - bottom);
		_position = glm::vec3(0, 0, 2);
		m_projectionMatrix = glm::ortho(left, right, bottom, top, nearZ, farZ);

		m_viewMatrix = glm::lookAt(_position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));


		m_combined = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::update(){
		if(m_needsUpdate){
			m_combined = m_projectionMatrix * m_viewMatrix;
			m_needsUpdate = false;
		}
	}

	void OrthographicCamera::resize(GLfloat newWidth, GLfloat newHeight){
		if(newWidth != _right || newHeight != _top){
			m_projectionMatrix = glm::ortho(_left, newWidth, _bottom, newHeight, _nearZ, _farZ);
			m_needsUpdate = true;
		}

	}


	void OrthographicCamera::translate(glm::vec3 translation) {
		m_viewMatrix = glm::translate(m_viewMatrix, translation);
		m_needsUpdate = true;


	}
	void OrthographicCamera::lookAt(glm::vec3 pos){
		m_viewMatrix = glm::lookAt(_position, pos, glm::vec3(0.0f, 1.0f, 0.0f));
		m_needsUpdate = true;
		VOFOG_CORE_WARN("{}:\tlookAt() not implemented.", __FILENAME__);

	}

	void OrthographicCamera::setCameraPosition(glm::vec3 position){
		_position = position;
		m_viewMatrix = glm::lookAt(position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_needsUpdate = true;
		VOFOG_CORE_WARN("{}:\tsetCameraPosition() not implemented.", __FILENAME__);
	}


}