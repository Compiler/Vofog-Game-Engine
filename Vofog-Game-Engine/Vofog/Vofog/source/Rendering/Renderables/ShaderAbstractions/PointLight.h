#pragma once
#include <Vofog.h>
#include <Rendering/Renderables/ShaderAbstractions/Light.h>

namespace Vofog{
	class VOFOG_API PointLight : public Vofog::Light{


	public:
		PointLight();
		virtual ~PointLight();
	};

}
