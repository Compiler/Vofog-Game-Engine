#pragma once
#include <Vofog.h>
#include <glad/glad.h>

namespace Vofog{
	class VOFOG_API RenderCommands{

	public:
		RenderCommands() = delete;
		static void clearColor(float r, float g, float b, float a = 1.0f){
			glClearColor(r, g, b, a);
			//GLFW_ACCUM_RED_BITS
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		/*static void setClearColor(float r, float g, float b, float a = 1.0f){
			_r = r; _g = g; _b = b; _a = a;
		}
		static void clear(){
			glClearColor(_r, _g, _b, _a);
			glClear(GL_COLOR_BUFFER_BIT);
		}*/
	};
}