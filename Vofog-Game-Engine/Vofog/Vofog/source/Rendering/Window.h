#pragma once

#include <Vofog.h>
#include <glad/glad.h>
//#include <VofogCore.h>
//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Vofog{

	class VOFOG_API Window{

	public:
		static GLfloat SCREEN_WIDTH;
		static GLfloat SCREEN_HEIGHT;

	private:

		//int _windowWidth, _windowHeight;
		std::string _windowName;
	
		GLFWwindow* _window; //shares memory with private pointer in Window.h

		friend class VofogCore;

		bool _resizeTriggered = false;

	public:
		Window(int width, int height, const char name[], bool fullscreen = false);

		//inline int getWidth() const { return _windowWidth; }
		//inline int getHeight()const { return _windowHeight; }

		void resetResizeFlag(){ _resizeTriggered = false; }
		bool windowResized(){ return _resizeTriggered; }
		~Window();
	};

}
