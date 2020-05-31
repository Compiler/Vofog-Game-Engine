#pragma once

#include <Vofog.h>
#include <Rendering/Renderables/Renderable.h>
#include <Rendering/Renderables/IndexBuffer.h>

namespace Vofog{


	class Sprite2D: public Vofog::Renderable{

	private:


	public:
		//Sprite2D();
		Sprite2D(float x, float y, float width, float height, const glm::vec4& color = glm::vec4(1, 1, 1, 1));
		Sprite2D(float x, float y, float z, float width, float height, const glm::vec4& color = glm::vec4(1, 1, 1, 1));


		~Sprite2D();


	};
}