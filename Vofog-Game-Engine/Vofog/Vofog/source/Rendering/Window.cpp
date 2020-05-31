#include <vpch.h>

#include "Window.h"

#include "wtypes.h"



GLfloat Vofog::Window::SCREEN_WIDTH;
GLfloat Vofog::Window::SCREEN_HEIGHT;


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	Vofog::Window::SCREEN_WIDTH = width;
	Vofog::Window::SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
}

Vofog::Window::Window(int width, int height, const char name[], bool fullscreen) : /*_windowWidth(width), _windowHeight(height),*/
	_windowName(name){
	if(!glfwInit()){
		exit(-4);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//version 4.6 Core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	if(fullscreen){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
		_window = glfwCreateWindow(mode->width, mode->height, name, NULL, NULL);
		Vofog::Window::SCREEN_WIDTH = mode->width;
		Vofog::Window::SCREEN_HEIGHT = mode->height;
	} else{

		Vofog::Window::SCREEN_WIDTH = width;
		Vofog::Window::SCREEN_HEIGHT = height;
		_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name, NULL, NULL);
	}
	
	
	

	//debug section for placing windows where i need them for debug purposes(and single monitor)
	#ifdef VOFOG_DEBUG_BUILD
		
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		//horizontal = desktop.right; vertical = desktop.bottom;
		//glfwSetWindowPos(_window, desktop.right - SCREEN_WIDTH, 0 + 45 );
	#endif
	
	glfwMakeContextCurrent(_window);
	if (_window == NULL || _window == nullptr){
		VOFOG_CORE_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return;
	}


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetWindowSizeCallback(_window, framebuffer_size_callback);

	
}





Vofog::Window::~Window(){
	glfwDestroyWindow(_window);
}
