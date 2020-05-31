//#include <vpch.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vofog.h"


#include <VofogCore.h>
#include <VofogCoreScripting.h>


void init();
void update();
void render();




















extern "C"{

	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>


}

int main(){




	#ifdef VOFOG_DEBUG_BUILD
	RECT desktop;
	HWND consoleWindow = GetConsoleWindow();
	GetWindowRect(consoleWindow, &desktop);
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;
	int consoleHeight = screenHeight / 3;
	int yPos = screenHeight;
	SetWindowPos(consoleWindow, 0, -1222, 400, 0, 0, 0x0001 | 0x0004);//left monitor
	//SetWindowPos(consoleWindow, 0, 2222, 400, 0, 0, 0x0001 | 0x0004);//right monitor
	//MoveWindow(consoleWindow, 0, yPos, 1920, consoleHeight, TRUE);
	//glfwSetWindowSize();
	HANDLE  hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	#endif

	Vofog::Window window(1920, 1080, "Vofog TRIAL Testing", true);


	Vofog::VofogCore core(window);
	 
	core.init();



	while (!core.windowClosed()) {
		core.update();
		//update();


		core.render();
		//render();

	}



	VOFOG_CORE_FATAL("Closing application...");
	core.onDetach();
	return 1;
}


void init() {



}

void update() {


}

void render() {


}
