#pragma once
#include <Vofog.h>
#include <Rendering/Renderables/BatchRenderer.h>

namespace Vofog{

	class VOFOG_API BatchRendererGui{

	private:
		const Vofog::BatchRenderer* _batch;
		
	public:

		BatchRendererGui(){}
		BatchRendererGui(const BatchRenderer* batch);

		
		void render(GLfloat elapsedTime = 0);

	};
	
}