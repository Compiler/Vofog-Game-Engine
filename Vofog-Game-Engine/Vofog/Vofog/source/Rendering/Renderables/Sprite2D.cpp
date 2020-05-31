#include <vpch.h>
#include "Sprite2D.h"

namespace Vofog{
	unsigned int Vofog::Renderable::CREATION_COUNT;

	Sprite2D::Sprite2D(float x, float y, float width, float height, const glm::vec4& color):
		Renderable(glm::vec3(x, y, 0), glm::vec3(width, height, 0), color){


		GLfloat vertices[12] = /*Transform position with model matrix in shader?*/
		{0, 0, 0,	width, 0, 0,	0, height, 0,	width, height, 0};

		GLfloat colors[12] =
		{color.x, color.y, color.z,color.x, color.y, color.z,color.x, color.y, color.z,color.x, color.y, color.z};

	}
	Sprite2D::Sprite2D(float x, float y, float z, float width, float height, const glm::vec4& color):
		Renderable(glm::vec3(x, y, z), glm::vec3(width, height, 0), color){


		GLfloat vertices[12] = /*Transform position with model matrix in shader?*/
		{0, 0, 0,	width, 0, 0,	0, height, 0,	width, height, 0};

		GLfloat colors[12] =
		{color.x, color.y, color.z,color.x, color.y, color.z,color.x, color.y, color.z,color.x, color.y, color.z};

	}



	Sprite2D::~Sprite2D(){
	}

}