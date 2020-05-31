#pragma once
#include <Vofog.h>
#include <tools/Events/Event.h>
namespace Vofog{
	class VOFOG_API EventListener{
	    public:
			virtual void onEvent() = 0;
	};
}