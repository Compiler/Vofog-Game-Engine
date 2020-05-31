#pragma once



#include <glad/glad.h>
#include "Vofog.h"
#include <tools/Logging.h>
#include <Rendering/Window.h>
#include <tools/Events/Event.h>
#include <tools/Events/EventMouse.h>
#include <tools/Layers/Layer.h>
#include <tools/Assets/ImageLoader.h>

#include <Rendering/Renderables/BatchRenderer.h>
#include <Rendering/Renderables/Sprite2D.h>
#include <Rendering/RenderCommands.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>


#include <tools/ImGui/ImGuiConsole.h>

#include <Rendering/Renderables/Cube.h>
#include <Rendering/Renderables/Texture.h>
#include <Rendering/TextureHandler.h>

#include <tools/Scenes/Scene.h>
#include <tools/Scenes/TestScene.h>
#include <tools/Scenes/DefaultScene.h>
#include <tools/Scenes/SceneManager.h>

#include <Entity Component System/EntityManager.h>
#include <Entity Component System/Components/Component.h>
#include <Entity Component System/Components/ScriptComponent.h>
#include <Entity Component System/Systems/EntitySystem.h>

#include <limits>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

#include <PlatformSpecific/OSFileSystem.h>

#include <tools/Events/EventManager.h>

#include <VofogEntity.h>
#include <VofogEntityManager.h>
#include <tools/LuaState.h>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
extern "C"{

	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>


}


CEREAL_REGISTER_DYNAMIC_INIT(VofogEngine);

//CEREAL_REGISTER_TYPE(Vofog::PerspectiveCamera);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Vofog::Camera, Vofog::PerspectiveCamera);
namespace Vofog{

	int INPUT_STATE = 0;

	class VOFOG_API VofogCore{
	#define FPS_250_MS (vfg_time_unit) 0.004
	#define FPS_60_MS (vfg_time_unit) 0.016
	#define MS_PER_FRAME FPS_250_MS
	#define FPS_MAX 25



	private:
		GLFWwindow* _window; //shares memory with private pointer in Window.h

		Vofog::ImGuiConsole* _imguiConsole = new Vofog::ImGuiConsole();

		std::unique_ptr<SceneManager> _sceneManger;
		std::unique_ptr<Vofog::ECS::EntityManager> _entityManager;
		std::string _projectFileName = "";
		bool _loadNewProject = false;
		bool _saveProject = false;

		vfg_time_unit _startTime = 0;
		vfg_time_unit _timeElapsed = 0;
		vfg_time_unit _frames = 0;
		vfg_time_unit _lag = 0;
		vfg_time_unit _Vofog_deltaTime = 0;
		vfg_time_unit _endTime = 0;
		vfg_time_unit _totalTime = 0;

		float lightColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float brightness = 0.0f;

		int _fpsIndex = -1;
		float _fpsValues[FPS_MAX] = {};

		bool _checkShader = false;
		bool _useGuiShader = true;
		bool _useRenderableShader = true;
		bool _useGUI = true;
		bool _postProcessing = false;
		bool _normalMapping = true;
		bool _visualizeTangentSpace = false;
		bool _useParallax = true;
		bool _useShadowVar = true;
		bool _useLighting = true;
		bool _useGammaModifier = true;

		Vofog::ECS::EntityID _currentEntity;
		std::shared_ptr<Vofog::ECS::EntityManager> _currentManager;

		glm::vec2 translation;
		bool setpos;


		void _setupImGui();
		void _guiRender();

		int scriptTrigger(lua_State* L);
		void vfg_unit_test();

		void _processSceneGui(Scene& currentScene);

		void _initScriptingState();

		bool playButtonPressed = false;

	public:

		ShaderProgram debugShaderProgram;


		VofogCore(const Window& windowRef);
		bool activeMouse = true;


		void onEvent();
		void init(); 
		void initConfigurationOptions();
		void render();
		void update();
		void onDetach();
		bool windowClosed();


		//inline GLfloat getWindowWidth()const{ return _screenWidth; }
		//inline GLfloat getWindowHeight()const{ return _screenHeight; }




		~VofogCore(){}
	};



	

}