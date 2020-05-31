#pragma once



#include <glad/glad.h>
#include "Vofog.h"
#include <tools/Logging.h>

#include <Rendering/TextureHandler.h>


#include <tools/ImGui/ImGuiConsole.h>

#include <tools/Events/EventManager.h>

#include <VofogEntity.h>
#include <VofogEntityManager.h>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


}

//CEREAL_REGISTER_DYNAMIC_INIT(VofogEngine);

//CEREAL_REGISTER_TYPE(Vofog::PerspectiveCamera);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Vofog::Camera, Vofog::PerspectiveCamera);
namespace Vofog {


	class VOFOG_API VofogCoreScripting {



	private:
		Vofog::ImGuiConsole* _imguiConsole = new Vofog::ImGuiConsole();


		std::shared_ptr<VofogEntityManager> _manager;

		void vfg_unit_test();
	public:


		VofogCoreScripting();
		bool activeMouse = true;


		void onEvent();
		void init();
		void initConfigurationOptions();
		void render();
		void update();
		void onDetach();
		bool windowClosed();



		~VofogCoreScripting() {}
	};





}