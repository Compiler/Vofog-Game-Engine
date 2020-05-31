#include "vpch.h"
#include "VofogCore.h"

#define FOR(x) for(int i = 0; i < x; i++)

int Vofog::ECS::Component::count = 0;

void vofog_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void vofog_mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void vofog_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

void load_new_vfg_project(std::string filepath, std::unique_ptr<Vofog::SceneManager>& sceneManager);


/*
/JMC /permissive- /Yu"vpch.h" /GS /W3 /Gy /Zc:wchar_t /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\outsourced\spdlog\include" /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\outsourced\glm" /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\Vofog\source\external code" /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\outsourced\cereal" /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\outsourced\bullet\src" /I"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\outsourced\Lua535\include" /ZI /Gm- /Od /sdl /Fd"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\intermediate-bin\Debug-windows-x64\VofogTesting\vc142.pdb" /Zc:inline /fp:precise /D "_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING" /D "_CRT_SECURE_NO_WARNINGS" /D "VOFOG_LIBRARY" /D "VOFOG_DEBUG_BUILD" /D "_WINDLL" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /RTCu /Gd /MDd /std:c++17 /FC /Fa"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\intermediate-bin\Debug-windows-x64\VofogTesting\" /EHsc /nologo /Fo"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\intermediate-bin\Debug-windows-x64\VofogTesting\" /Fp"C:\Users\User\Desktop\Vofog Trial\VofogEngine\Vofog\intermediate-bin\Debug-windows-x64\VofogTesting\VofogEngine.pch" /diagnostics:column


*/
//abstract to windows context
/*char* test_open_file(){
	char* modelFileName;
	char bufFileName[MAX_PATH * 2] = "";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPCSTR)TEXT("3D model (*.model)\0*.model\0 All files (*.*)\0*.*\0");
	ofn.lpstrFile = (LPSTR)bufFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPCSTR)TEXT("Choose 3D model file");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)TEXT("*.model");

	if(GetOpenFileName(&ofn)){
		//delete_zero_symbols(bufFileName);
		modelFileName = (char*)calloc(strlen(bufFileName) + 1, sizeof(char));
		if(modelFileName != 0)
			strcpy(modelFileName, bufFileName);
		else
			VOFOG_CORE_ERROR("MFN: {} = 0", modelFileName);
		return modelFileName;
	} else{
		VOFOG_CORE_ERROR("Error");
		return NULL;
	}
}
*/


const char* getGLName(GLuint val){
		if(val == GL_FLOAT)
			return "float";
		else if(val == GL_SAMPLER_2D)
			return "sampler2D";

		return "No matching type";
}
//
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
	std::string errorDetails = "Other Error";
	if(type == GL_DEBUG_TYPE_ERROR)
		errorDetails = "General Error";
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
		errorDetails = "Deprecated Behavior Error";
	else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		errorDetails = "Undefined Behavior Error";
	else if(type == GL_DEBUG_TYPE_PORTABILITY)
		errorDetails = "Portability Error";
	else if(type == GL_DEBUG_TYPE_PERFORMANCE)
		errorDetails = "Performance Error";

	std::string errorSource = "Other source";
	if(source == GL_DEBUG_SOURCE_API)
		errorSource = "OpenGL API";
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
		errorSource = "Window-Systems API";
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER)
		errorSource = "Shader Compiler";
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY)
		errorSource = "Third-Party OpenGL";
	else if(source == GL_DEBUG_SOURCE_APPLICATION)
		errorSource = "Vofog";


	if(severity == GL_DEBUG_SEVERITY_MEDIUM){//ignore pedantic errors/msgs

		VOFOG_CORE_FATAL("{}:{}\tOpenGL {}\tSeverity: Medium. \t ID: {}\n\t\"{}\"\n", __FILENAME__, __LINE__, errorDetails, id, message);
		//DebugBreak();
	}
	if(severity == GL_DEBUG_SEVERITY_LOW){//ignore pedantic errors/msgs
		VOFOG_CORE_FATAL("{}:{}\tOpenGL {}\tSeverity: Low. \t ID: {}\n\t\"{}\"\n", __FILENAME__, __LINE__, errorDetails, id, message);


	//	DebugBreak();
	}
}


Vofog::VofogCore::VofogCore(const Window& windowRef){
	_window = windowRef._window;
}

void Vofog::VofogCore::initConfigurationOptions() {
	//btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	Vofog::Logging::init(_imguiConsole);
	VOFOG_CORE_TRACE("Init Core");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glEnable(GL_BLEND);
#define LAMBDA GL_SRC_ALPHA
#define ONE_MINUS_LAMBDA GL_ONE_MINUS_SRC_ALPHA
	glBlendFunc(LAMBDA, ONE_MINUS_LAMBDA);//over operation
	//glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilMask(0xFF); // each bit is written to the stencil buffer as is
		//glStencilMask(0x00);
	//	glStencilFunc(GL_EQUAL, 1, 0xFF);


	//TODO: re-enable: disabled backface culling for time being
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/


	//glEnable(GL_FRAMEBUFFER_SRGB); 


	//glEnable(GL_MULTISAMPLE);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glad_glPointSize(10);

	glfwSetWindowUserPointer(_window, this);
	glfwSwapInterval(0);


}

void Vofog::VofogCore::_initScriptingState() {
	LuaState::lua->open_libraries(sol::lib::base);
	sol::usertype<glm::vec3> vec3_type = LuaState::lua->new_usertype<glm::vec3>("vec3", sol::constructors<glm::vec3()>());
	vec3_type["x"] = &glm::vec3::x;vec3_type["y"] = &glm::vec3::y;vec3_type["z"] = &glm::vec3::z;
	sol::usertype<Vofog::ECS::TransformComponent> transform_type = LuaState::lua->new_usertype<Vofog::ECS::TransformComponent>("TransformComponent", sol::constructors<VofogTransformComponent()>());
	transform_type["position"] = &Vofog::ECS::TransformComponent::position;
	transform_type["rotation"] = &Vofog::ECS::TransformComponent::rotation;
	transform_type["size"] = &Vofog::ECS::TransformComponent::size;
	ECS::Entity e1, e2, e3, e4;
	VOFOG_CORE_LOG(e1.toString());
	VOFOG_CORE_LOG(e2.toString());
	VOFOG_CORE_LOG(e3.toString());
	VOFOG_CORE_LOG(e4.toString());
	sol::usertype<ECS::Entity> entity_type = LuaState::lua->new_usertype<ECS::Entity>("Entity", sol::constructors<VofogEntity(), VofogEntity(const char*)>());
	entity_type["getID"] = &ECS::Entity::getID;
	entity_type["getName"] = &ECS::Entity::getName;
	entity_type["setName"] = &ECS::Entity::setName;
	entity_type["toString"] = &ECS::Entity::toString;
	

	sol::usertype<Vofog::EventManager> input_type = LuaState::lua->new_usertype<Vofog::EventManager>("Input");
	input_type["isMouseButtonDown"] = &Vofog::EventManager::isMousePressed;



	sol::usertype<Vofog::ECS::EntityManager> ecs_type = LuaState::lua->new_usertype<Vofog::ECS::EntityManager>("EntityManager");
	ecs_type["getCurrentEntity"] = &Vofog::ECS::EntityManager::getCurrentEntity;
	ecs_type["getTransform"] = &Vofog::ECS::EntityManager::getTransformComponent;



	LuaState::lua->script_file((VOFOG_DEFAULT_IMAGE_FILEPATH + "../scripts/lua/SolTestScript.lua").c_str());
	
	//std::cin.get();
}


#include <cereal/archives/binary.hpp>
#include <sstream>

void Vofog::VofogCore::init() {
	//test();
	initConfigurationOptions();
	//_initScriptingState();
	
	Vofog::LuaState::initialize();


	//////////////////
	GLint textureUnits;
 	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	VOFOG_CORE_LOG("Available Texture slots: {}", textureUnits);

	TextureHandler::_init();
	ScriptHandler::populate_all_scripts();
	TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "cheapbricks_diffuse.jpg", "SRGB bricks", GL_SRGB, GL_RGB);//srgb
	TextureHandler::add(VOFOG_DEFAULT_IMAGE_FILEPATH + "container.png", "SRGBA container", GL_SRGB_ALPHA, GL_RGBA);//srgba
	///
	if(true){
		std::ifstream myfile;
		myfile.open("recentProject.txt");
		if (myfile.is_open()) std::getline(myfile, _projectFileName);
		else {
			VOFOG_CORE_ERROR("Couldn't read recent file.");
			std::cin.get();
			std::exit(1);
		}
		myfile.close();
		if (_projectFileName.empty()) {
			_sceneManger = std::make_unique<SceneManager>();
			std::shared_ptr<Scene> scene = std::make_shared<DefaultScene>();
			scene->initialize();
			_sceneManger->addScene(scene);
		}
		else {
			std::ifstream iss(_projectFileName, std::ios::in | std::ios::binary);
			cereal::BinaryInputArchive iarchive(iss);
			iarchive(_sceneManger);
		}
	} else{
		_sceneManger = std::make_unique<SceneManager>();
		std::shared_ptr<Scene> scene = std::make_shared<DefaultScene>();
		scene->initialize();
		_sceneManger->addScene(scene);


	}


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigDockingWithShift = false;
	io.ConfigDockingAlwaysTabBar = true;
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_WindowBg].w = 1;
	style->WindowRounding = 0;

	//TODO: fix this dog shit color scheme.. the above is fine
	/*style->Colors[ImGuiCol_Border] = ImVec4(0.7f, 0, 1, 0.3f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7f, 0, 1, 0.3f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7f, 0, 1, 0.5f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.7f, 0, 1, 0.9f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.7f, 0, 0.7f, 0.5f);*/
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui_ImplGlfw_InitForOpenGL(_window, true);

	glfwSetKeyCallback(_window, vofog_key_callback);
	glfwSetCursorPosCallback(_window, vofog_cursor_pos_callback);


	glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset){
		VofogCore* windowPointer = (VofogCore*)glfwGetWindowUserPointer(window);
		windowPointer->onEvent();
		ImGuiIO& io = ImGui::GetIO();
		const static float mag = 70.0f;
		io.MouseWheelH += (float)xoffset * io.DeltaTime * mag;
		io.MouseWheel += (float)yoffset* io.DeltaTime * mag;
		
	});



	glfwSetMouseButtonCallback(_window, vofog_mouse_click_callback);
	
	



	translation.x = 0;
	translation.y = 0;


	debugShaderProgram.init("DEBUG/BulletDrawLine.vert", "DEBUG/BulletDrawLine.frag", "DEBUG/BulletDrawLine.geom");
	//std::string openName = vfg_open_file();
	//VOFOG_CORE_DEBUG("Saved to {}", openName);
	////_sceneManger->onDetach();
	//if(openName != ""){
	//	_projectFileName = openName;
	//	_loadNewProject = true;
	//}


}










void Vofog::VofogCore::render(){

	_sceneManger->render();
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	

	
	_guiRender();





	glfwSwapBuffers(_window);

	/*End frame here*/
	_frames++;

	_endTime = glfwGetTime();

	if(_loadNewProject){
		//delete _sceneManger.release();
		std::ifstream iss(_projectFileName, std::ios::in | std::ios::binary);
		cereal::BinaryInputArchive iarchive(iss);
		iarchive(_sceneManger);

		_loadNewProject = false;
		iss.close();
	}

	if(_saveProject){
		std::ofstream oss(_projectFileName, std::ios::out | std::ios::binary);
		cereal::BinaryOutputArchive oarchive(oss);
		oarchive(_sceneManger);
		_saveProject = false;
		oss.close();

	}


}

void Vofog::VofogCore::_guiRender(){

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if(_useGUI)
		_setupImGui();

	ImGui::Render();
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


void Vofog::VofogCore::onEvent(){

	_sceneManger->onEvent();

}
void Vofog::VofogCore::update(){
	static double timer = glfwGetTime();
	double current = glfwGetTime() - timer;
	if(current > 20){
		ScriptHandler::populate_all_scripts();
	}

	_timeElapsed = _endTime - _startTime; _totalTime += _timeElapsed;

	//60 frames / second = 60 frames / 1000 ms = 0.06 frames / 1 ms, 16.6 ms / frame

	_startTime = glfwGetTime();
	//Sleep(16.66666666666666667f - _timeElapsed);

	_lag += _timeElapsed;


	//this hack fixes weird offset for window
	static bool hackFix = false;
	if(!hackFix){
		glfwRestoreWindow(_window);
		glfwMaximizeWindow(_window);
		//glBindTexture(GL_TEXTURE_2D, _frameTexture.getTextureID());
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Vofog::Window::SCREEN_WIDTH, Vofog::Window::SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//glBindTexture(GL_TEXTURE_2D, 0);
		hackFix = true;
	}


	/*Start frame here*/
	if(_totalTime >= 0.5f){
		_totalTime = 0;
		_fpsIndex++;
		if(_fpsIndex == FPS_MAX)
			_fpsIndex = 0;
		_fpsValues[_fpsIndex] = _frames * 2;

		//VOFOG_CORE_DEBUG("{} : {}", _frames, ImGui::GetIO().Framerate);
		_frames = 0;

	}

	glfwPollEvents();
	_sceneManger->update();
	if (EventManager::getInstance().isKeyReleased(VofogKeyStore::VOFOG_KEY_LEFT_CONTROL))
		playButtonPressed = !playButtonPressed;
	if(playButtonPressed){
		if(_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getState() == ECS::EngineStateTuple::EDITOR)
			_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->setState(ECS::EngineStateTuple::APPLICATION);
		else if(_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getState() == ECS::EngineStateTuple::APPLICATION)
			_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->setState(ECS::EngineStateTuple::EDITOR);
		playButtonPressed = false;
	}

	//(*Vofog::LuaState::lua)["currentECS"] = manager;

	(*Vofog::LuaState::lua)["ecs"] = _sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager;
	//Vofog::LuaState::lua->script_file((VOFOG_DEFUALT_RES_FILEPATH + "scripts/lua/LeftClickFire.lua").c_str());
	//
	if (EventManager::getInstance().isKeyPressed(VofogKeyStore::VOFOG_KEY_B)) {
		auto id= _sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getEntityByName("Player");
		auto physics = _sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getComponent<ECS::PhysicsComponent>(id);
		physics->body->applyImpulse(btVector3(10,0,0), btVector3(1, 0, 0));
		physics->body->setFriction(1);
	}

	EventManager::getInstance().clear();


}

		



void save_project(std::unique_ptr<Vofog::SceneManager>& manager){
	ImGui::BeginPopupModal("Enter a filename");
	ImGui::EndPopup();
	{
		//std::ofstream os("Application.vfg");
		//cereal::BinaryOutputArchive archive(os);
		//archive(manager);
	}



}



void Vofog::VofogCore::_setupImGui(){


	static float lightColorSlider = 0;
	//ImGui::ShowMetricsWindow();
	//_layers[0]->translateCamera(glm::vec3(translation.x * _timeElapsed * 100,translation.y * _timeElapsed * 100,0));

	GLfloat windowWidths = Vofog::Window::SCREEN_WIDTH / 6;
	static GLfloat windowTopDisplace = 0;
	if(ImGui::BeginMainMenuBar()){
		windowTopDisplace = ImGui::GetWindowHeight();

		if(ImGui::BeginMenu("File")){
			if(ImGui::BeginMenu("New")){
				if(ImGui::MenuItem("Project", "CTRL+SHIFT+O"))
					VOFOG_CORE_DEBUG("{}", vfg_open_file());

				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Open", "CTRL+SHIFT+O")){
				std::string openName = vfg_open_file();
				VOFOG_CORE_DEBUG("{}", openName);

				if(openName != ""){
					_projectFileName = openName;
					_loadNewProject = true;
				}


			}

			if(ImGui::MenuItem("Save", "CTRL+S")){
				if(_projectFileName == ""){
					std::string openName = vfg_open_file();
					VOFOG_CORE_DEBUG("Saved to {}", openName);
					//_sceneManger->onDetach();
					if(openName != ""){
						_projectFileName = openName;
						_saveProject = true;
					}
				} else{
					VOFOG_CORE_DEBUG("Save location exists at {}, saving to there!", _projectFileName);
					if(true){
						_saveProject = true;
					}
				}
			}

			if(ImGui::MenuItem("Save as", "CTRL+SHIFT+S")){
				_projectFileName = vfg_save_file();
				VOFOG_CORE_DEBUG("Saved to {}", _projectFileName);
				if(true){
					/*std::ofstream oss(_projectFileName, std::ios::out | std::ios::binary);
					cereal::BinaryOutputArchive oarchive(oss);
					oarchive(_sceneManger);
					oss.close();*/
					_saveProject = true;
				}

			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Options")){
			if(ImGui::MenuItem("Undo", "CTRL+Z")){}
			if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)){}  // Disabled item
			ImGui::Separator();
			if(ImGui::MenuItem("Cut", "CTRL+X")){}
			if(ImGui::MenuItem("Copy", "CTRL+C")){}
			if(ImGui::MenuItem("Paste", "CTRL+V")){}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Exit without saving")) {
			std::exit(-4);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	//ImGui::DockSpace(ImGui::GetID("Vofog"), ImVec2(0,0), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_thruCentralde);
	ImVec2 cPos = ImVec2(0, (Vofog::Window::SCREEN_HEIGHT - (Vofog::Window::SCREEN_HEIGHT / 6.0f) + 5.0f));
	ImVec2 cSize = ImVec2(Vofog::Window::SCREEN_WIDTH / 3, (Vofog::Window::SCREEN_HEIGHT / 6.0f));
	_imguiConsole->Draw("Vofog Console", 0, cPos.x, cPos.y, cSize.x, cSize.y);
	ImGui::SetNextWindowPos(ImVec2(cSize.x, cPos.y));
	ImGui::SetNextWindowSize(ImVec2(Vofog::Window::SCREEN_WIDTH - cSize.x - windowWidths, cSize.y));
	ImGui::Begin("Texture Slots");
	ImVec4 border(0.5f, 0.5f, 0.7f, 0.7f);
	unsigned int sep = TextureHandler::getSize() + 1;
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(20, 20));
	float w = ImGui::GetContentRegionAvail().x;
	float h = ImGui::GetContentRegionAvail().y;
	float mag = 16;
	float width = w / mag;
	float height = (h / mag) * (w / h);
	float roomLeft = width * mag;
	for(int i = 0; i < sep; i++){
		roomLeft -= width;
		ImGui::Image((void*)(intptr_t)i, ImVec2(width, height),
			ImVec2(0, 0), ImVec2(1, -1), ImVec4(1, 1, 1, 1), border);
		if(roomLeft > width)
			ImGui::SameLine();
		else
			roomLeft = width * mag;

	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();



	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, ImVec4(0, 0, 0, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TabActive, ImVec4(0.4f, 0, 1, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Tab, ImVec4(0.2f, 0, 0.5f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TabHovered, ImVec4(0.5f, 0, 1, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_DockingPreview, ImVec4(1.0f, 0, 0, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TabUnfocused, ImVec4(0.0f, 0, 1, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Border, ImVec4(0.1f, 0, 0.8f, 0.8f));

	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize, 0.01);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ChildBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabRounding, 0);
	//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(0,0));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	
	
	ImGui::SetNextWindowPos(ImVec2(windowWidths, windowTopDisplace));
	float frameWindowWidth = Vofog::Window::SCREEN_WIDTH - (2 * windowWidths);
	float frameWindowHeight = Vofog::Window::SCREEN_HEIGHT - (cSize.y + windowTopDisplace);
	ImGui::SetNextWindowSize(ImVec2(frameWindowWidth, frameWindowHeight));
	ImGui::Begin("Scenes", 0);
		
	std::shared_ptr<Scene> nextScene = _sceneManger->getCurrentScene();
	if(ImGui::BeginTabBar("##rendererTabs", ImGuiTabBarFlags_Reorderable)){
		auto it = _sceneManger->scenes();
		int count = 0;
		for(; it.first != it.second; it.first++){
			if(ImGui::BeginTabItem((*it.first)->getName().c_str())){
				std::string label = "##Scene" + (*it.first)->getName();
				(*it.first)->bindFrame();
				/*VOFOG_CORE_LOG("{}, {}", frameWindowHeight,
					(*it.first)->getFrameTexture());*/
				float xx = ImGui::GetWindowContentRegionMax().x;
				float yy = ImGui::GetContentRegionAvail().y - (0.001 * ImGui::GetContentRegionAvail().y);
				ImGui::Image((void*)(intptr_t)(*it.first)->getFrameTexture(), ImVec2(xx,yy),
					ImVec2(0, 0), ImVec2(1, -1), ImVec4(1, 1, 1, 1), ImVec4(0.2f, 0, 0.5f, 0.7f));
				auto i = ImGui::GetWindowPos();
				nextScene = ((*it.first));
				_sceneManger->setScene(nextScene);

				/*TODO: make this way more secure... this a massive function*/
				if (ImGui::IsWindowHovered()) Vofog::INPUT_STATE = 0;
				else Vofog::INPUT_STATE = 1;
				ImGui::EndTabItem();

			}
		}

		if (_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getState() != ECS::EngineStateTuple::APPLICATION){
			if (ImGui::BeginPopupContextWindow("item context menu")) {
				if (ImGui::MenuItem("Add new Scene")) {
					std::shared_ptr<Scene> scene = std::make_shared<DefaultScene>();
					scene->initialize();
					_sceneManger->addScene(scene);
				}

				ImGui::EndPopup();
			}
		}



		ImGui::EndTabBar();

	}
	//ImGui::Begin("Images");
	/*ImGui::BeginChild("Container.jpg", ImVec2(0, 0), false, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
	ImGui::Image((void*)(intptr_t)2, ImVec2(Vofog::Window::SCREEN_WIDTH - (2 * windowWidths), Vofog::Window::SCREEN_HEIGHT - cSize.y),
		ImVec2(0, 0), ImVec2(1, -1));

	ImGui::EndChild();*/
	//ImGui::End();
	//ImGui::Begin("Scene 1");

	//ImGui::End();
	ImGui::End();



	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();


	ImGui::SetNextWindowPos(ImVec2(0, windowTopDisplace), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(windowWidths, Vofog::Window::SCREEN_HEIGHT - (cSize.y + windowTopDisplace)), ImGuiCond_Once);
	ImGui::Begin("Main Functions Menu");
	/*FPS VALUES*/
	ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.7f, 1), "FPS: (%.1f)", _fpsValues[_fpsIndex]);
	//ImGuiIO& io = ImGui::GetIO();
	ImGui::SameLine();
	ImGui::Text("%.3f ms/frame (%.5f Delta)", 1000.0f / _fpsValues[_fpsIndex], _timeElapsed);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotLines, ImVec4(1, 0, 1, 1));
	ImGui::PlotLines("##LineFrameTimes", _fpsValues, IM_ARRAYSIZE(_fpsValues), 0, 0, 0, 500);
	ImGui::PlotHistogram("##HistFrameTimes", _fpsValues, IM_ARRAYSIZE(_fpsValues), 0, 0, 0, 500);
	ImGui::PopStyleColor();

	ImGui::Separator();
	/*CONSOLE*/
	/*Left side shader drop down menu*/
	ImGui::Checkbox("Show Demo", &_checkShader);
	if(_checkShader){
		ImGui::ShowDemoWindow();
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0.4, 0, 1, 0.8));


	ImGui::PopStyleColor();

	ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 30, 0);
	
	if(_sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager->getState() == ECS::EngineStateTuple::APPLICATION)ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0.5, 0.7, 0.5, 0.8));
	else ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0.1, 0.7, 0.1, 0.8));
	if (ImGui::Button("Start")) {
		playButtonPressed = !playButtonPressed;
	}
	ImGui::PopStyleColor();
	ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 10, 0);if (ImGui::Button("Debug")) {
	}



	if(ImGui::CollapsingHeader("Models##Temp")){



	}

	/*======================================================================================================
	  =====================================Left First Option================================================
	  ======================================================================================================*/

	if(ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)){



		//_sceneManger->setScene(nextScene);
	}

	if(ImGui::CollapsingHeader("Scripting##MainScripting")){
		static std::string currentScriptName = "No scripts";
		static bool changedFiles = false;
		
		if (ScriptHandler::scripts.empty()) {
			if (currentScriptName != "No scripts") {
				currentScriptName = "No scripts";
			}
		} else {
			if (currentScriptName == "No scripts") {
				currentScriptName = ScriptHandler::scripts.front();
				changedFiles = true;
			}
		}
		if (ImGui::BeginCombo("Scripts", currentScriptName.substr(currentScriptName.find_last_of('/') + 1).c_str())) {
			for (int i = 0; i < ScriptHandler::scripts.size(); i++) {
				std::string file = ScriptHandler::scripts[i];
				bool selected = (file == currentScriptName);
				std::string readableFileName = file.substr(file.find_last_of('/') + 1);
				if (ImGui::Selectable(readableFileName.c_str(), selected)) {
					currentScriptName = file;
					changedFiles = true;
				}
			}
			ImGui::EndCombo();
		}
		//read in file
		static constexpr int sizeOfScriptFile = 1024;
		static char thing[sizeOfScriptFile];
		if (changedFiles) {
			memset(thing, 0, sizeOfScriptFile * (sizeof thing[0]));
			VOFOG_CORE_DEBUG("Files changed");
			std::ifstream in(currentScriptName.c_str());
			in.read((char*)&thing, sizeof(thing));
			changedFiles = false;
		}
		/*if(ImGui::Button("Load##MainScriptLoad")){
			std::string name = vfg_save_text_file();
			std::ifstream in(name.c_str());
			in.read((char*)&thing, sizeof(thing));
		}*/
		ImGui::SameLine();
		if(ImGui::Button("Save##MainScriptSave")){
		//	std::string name = vfg_save_text_file();
			std::ofstream out(currentScriptName.c_str());
			int scriptSize = 0;
			while(thing[scriptSize] != '\0')
				scriptSize++;
			out.write((char*)thing, scriptSize);
		}
		if (ImGui::GetIO().KeysDown[Vofog::VofogKeyStore::VOFOG_KEY_LEFT_CONTROL] && ImGui::GetIO().KeysDown[Vofog::VofogKeyStore::VOFOG_KEY_S]) {
			static double saveStartTime = -1.1;
			static double currentElapsed = 0;
			currentElapsed = glfwGetTime() - saveStartTime;
			//VOFOG_CORE_LOG("Attempting to save script file... {}", currentElapsed);
			if (currentElapsed > 1) {
				saveStartTime = glfwGetTime();
				std::ofstream out(currentScriptName.c_str());
				int scriptSize = 0;
				while (thing[scriptSize] != '\0')
					scriptSize++;
				out.write((char*)thing, scriptSize);
				VOFOG_CORE_DEBUG("Saved script file!");
			}
		}
		if (ImGui::IsWindowFocused()) {
			
			/*if (EventManager::isKeyPressed(VofogKeyStore::VOFOG_KEY_LEFT_CONTROL) && EventManager::isKeyReleased(VofogKeyStore::VOFOG_KEY_S)) {
				VOFOG_CORE_LOG("Saved script file?");
			}*/
			
		}
		ImGui::InputTextMultiline("##source", thing, IM_ARRAYSIZE(thing), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);

	}


	if(ImGui::CollapsingHeader("Collision System##MainEvents")){

	}

	if(ImGui::CollapsingHeader("Physics System##MainEvents")){

	}

	static glm::vec4 selected(0);
	//textureIDs = selected;
	if (ImGui::CollapsingHeader("Sprite Componenet", ImGuiTreeNodeFlags_DefaultOpen)) {
		{
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" , "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a"};
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 4.0f);
			ImGui::Text("Lists:");
			static int selection[4] = { 0, 1, 2, 3 };
			static int select = 0;
			for (int i = 0; i < 4; i++) {
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				//ImGui::ListBoxHeader("sup");
				ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
				ImGui::PopID();
				//if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
			}
			ImGui::PopItemWidth();
		}


		static float width;
		static ImVec2 dims;
		width = ImGui::GetContentRegionAvailWidth();
		dims = ImVec2(width / 5.0f, width / 5.0f);
		float spacing = (width * 0.2f) / 4.0;





		ImGui::Text("First"); ImGui::SameLine(dims.x / 2.0f); ImGui::Text("Second"); //ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)selected.x, dims); ImGui::SameLine(0, spacing);
		ImGui::Image((void*)(intptr_t)selected.y, dims); ImGui::SameLine(0, spacing);
		ImGui::Image((void*)(intptr_t)selected.z, dims); ImGui::SameLine(0, spacing);
		ImGui::Image((void*)(intptr_t)selected.w, dims);

		static glm::vec4 images(0);
		static std::vector<std::string>& imagePaths = TextureHandler::imagePaths;
		static std::vector<std::string>& imageReadableNames = TextureHandler::imageReadableNames;
		static bool changedFiles = false;

		ImGui::Dummy(ImVec2(10,10));
		ImGui::PushItemWidth(dims.x);

		ImGui::SameLine();
		if (ImGui::BeginCombo("##SpriteComponentImage1", imageReadableNames[images.x].c_str())) {
			for (int i = 0; i < imageReadableNames.size(); i++) {
				bool selected = (i == images.x);
				if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
					images.x = i;
					changedFiles = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo("##SpriteComponentImage2", imageReadableNames[images.y].c_str())) {
			for (int i = 0; i < imageReadableNames.size(); i++) {
				bool selected = (i == images.y);
				if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
					images.y = i;
					changedFiles = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo("##SpriteComponentImage3", imageReadableNames[images.z].c_str())) {
			for (int i = 0; i < imageReadableNames.size(); i++) {
				bool selected = (i == images.z);
				if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
					images.z = i;
					changedFiles = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo("##SpriteComponentImage4", imageReadableNames[images.w].c_str())) {
			for (int i = 0; i < imageReadableNames.size(); i++) {
				bool selected = (i == images.w);
				if (ImGui::Selectable(imageReadableNames[i].c_str(), selected)) {
					images.w = i;
					changedFiles = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Dummy(ImVec2(10,10));

		for (int n = 0; n < imagePaths.size(); n++) {
			char buf[64];
			sprintf(buf, imagePaths[n].c_str(), n);
			ImGui::PushItemWidth(dims.x);
								if (ImGui::Selectable((imageReadableNames[n] + "##xImageSelection").c_str(), selected.x == n + 1, 0, ImVec2(dims.x, 0))) selected.x = n + 1;
			ImGui::SameLine(0, spacing);  if (ImGui::Selectable((imageReadableNames[n] + "##yImageSelection").c_str(), selected.y == n + 1, 0, ImVec2(dims.x, 0))) selected.y = n + 1;
			ImGui::SameLine(0, spacing);  if (ImGui::Selectable((imageReadableNames[n] + "##zImageSelection").c_str(), selected.z == n + 1, 0, ImVec2(dims.x, 0))) selected.z = n + 1;
			ImGui::SameLine(0, spacing);  if (ImGui::Selectable((imageReadableNames[n] + "##wImageSelection").c_str(), selected.w == n + 1, 0, ImVec2(dims.x, 0))) selected.w = n + 1;
			ImGui::PopItemWidth();
		}


	}



	ImGui::End();



	
	
	ImGui::SetNextWindowPos(ImVec2(Vofog::Window::SCREEN_WIDTH - windowWidths, windowTopDisplace), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(windowWidths, Vofog::Window::SCREEN_HEIGHT - windowTopDisplace), ImGuiCond_Once);
	ImGui::Begin(("Scene: " + _sceneManger->getCurrentScene()->getName()).c_str());
	auto entityManager = _sceneManger->getCurrentScene()->m_sceneLayers[0]->m_entityManager;
	static ECS::EntityID currentEntityID = 0;
	currentEntityID = entityManager->getCurrentEntity();
	static bool deleted = false;
	_currentEntity = currentEntityID;
	auto entityIDs = entityManager->getIDS();
	if(entityIDs.size() > 0){
		auto e = entityManager->getEntity(currentEntityID);
		std::string currentName = entityManager->getEntity(currentEntityID)->getName();

		std::string buttonLabel = ("X##" + currentName);
		deleted = false;
		if (ImGui::Button(buttonLabel.c_str())) {
			//entityManager->removeEntity(currentEntityID);
			deleted = true;
		}
		ImGui::SameLine();
		if(ImGui::BeginCombo("Entities", currentName.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for(int i = 0; i < entityIDs.size(); i++){
				bool is_selected = (currentEntityID == entityIDs[i]);

				if(ImGui::Selectable(entityManager->getEntity(entityIDs[i])->getName().c_str(), is_selected)){
					currentEntityID = entityIDs[i];
					ImGui::Text(entityManager->getEntity(entityIDs[i])->getName().c_str());
				}
				if(is_selected)
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			}
			ImGui::EndCombo();
		}
		entityManager->setCurrentEntity(currentEntityID);
	}

	//entityManager->getEntityName(currentEntityID);
	static char buf[128];
	strcpy(buf, entityManager->getEntityName(currentEntityID).c_str());
	if (ImGui::InputText("new name here!", buf, 128)) {
		VOFOG_CORE_LOG("True: {}", buf);
		entityManager->getEntity(currentEntityID)->setName(buf);
	}
	static std::vector<ECS::ComponentID> compIds = entityManager->getComponentIDs();
	compIds = entityManager->getComponentIDs();
	for(int i = 0; i < compIds.size(); i++){
		auto thing = entityManager->getComponent(currentEntityID, compIds[i]);
		
		if(thing){
			std::string buttonLabel = ("X##" + std::to_string(compIds[i]));
			if(ImGui::Button(buttonLabel.c_str())){
				entityManager->removeComponent(currentEntityID, compIds[i]);
				continue;
			}
			ImGui::SameLine();
			//VOFOG_CORE_LOG("{}", thing->getID());
			thing->_display_gui_information_();
		}


	}
	
	
	if(ImGui::BeginPopupContextWindow("item context menu")){
		if (ImGui::MenuItem("Add new Entity")) {
			entityManager->addEntity();
		}

		if (ImGui::MenuItem("Add new complete Entity")) {
			auto id = entityManager->addEntity();
			entityManager->attachComponent<ECS::TransformComponent>(id, ECS::TransformComponent());
			entityManager->attachComponent<ECS::RenderableComponent>(id, ECS::RenderableComponent());
			entityManager->attachComponent<ECS::MeshRendererComponent>(id, ECS::MeshRendererComponent());
			entityManager->attachComponent<ECS::MeshTypeComponent>(id, ECS::MeshTypeComponent());
			ECS::PhysicsComponent physicsComp;
			physicsComp.mass = 1.0f;
			auto trans = entityManager->getComponent<ECS::TransformComponent>(id);
			if (!trans) {
				*trans = ECS::TransformComponent();
				trans->size = glm::vec3(1);
				entityManager->attachComponent<ECS::TransformComponent>(id, trans);
			}
			Physics::addBox(id, *trans, 1, entityManager->getWorld(), entityManager);
			Physics::attachPlane(id, *trans, entityManager->getWorld(), _sceneManger->getCurrentScene()->m_sceneLayers[0]->getCamera()->getDirection(), entityManager);
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Add new component")){
			if(ImGui::MenuItem("Transform Component")){
				entityManager->attachComponent<ECS::TransformComponent>(currentEntityID, ECS::TransformComponent());
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Renderable Component")){
				entityManager->attachComponent<ECS::RenderableComponent>(currentEntityID, ECS::RenderableComponent());
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Mesh Renderer Component")){
				entityManager->attachComponent<ECS::MeshRendererComponent>(currentEntityID, ECS::MeshRendererComponent());
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Mesh Type Component")){
				entityManager->attachComponent<ECS::MeshTypeComponent>(currentEntityID, ECS::MeshTypeComponent());
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Sprite Component")) {
				entityManager->attachComponent<ECS::SpriteComponent>(currentEntityID, ECS::SpriteComponent());
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Script Component")) {
				ECS::ScriptComponent comp = ECS::ScriptComponent();
				comp.filepath = (VOFOG_DEFAULT_IMAGE_FILEPATH + "../scripts/lua/LeftClickFire.lua");
				entityManager->attachComponent<ECS::ScriptComponent>(currentEntityID, comp);
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Physics Component")){
				ECS::PhysicsComponent physicsComp;
				physicsComp.mass = 1.0f;
				auto trans = entityManager->getComponent<ECS::TransformComponent>(currentEntityID);
				if(!trans){
					*trans = ECS::TransformComponent();
					trans->size = glm::vec3(1);
					entityManager->attachComponent<ECS::TransformComponent>(currentEntityID, trans);
				}
				Physics::addBox(currentEntityID, *trans, 1, entityManager->getWorld(), entityManager);
				Physics::attachPlane(currentEntityID, *trans, entityManager->getWorld(), _sceneManger->getCurrentScene()->m_sceneLayers[0]->getCamera()->getDirection(), entityManager);
				//entityManager->attachComponent<ECS::PhysicsComponent>(currentEntityID, ECS::PhysicsComponent());
			}

			ImGui::EndMenu();

		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Add Lighting")) {
			if (ImGui::MenuItem("Point Light Component")) {
				entityManager->attachComponent<ECS::PointLightComponent>(currentEntityID, ECS::PointLightComponent());
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Spot Light Component")) {
				entityManager->attachComponent<ECS::PointLightComponent>(currentEntityID, ECS::PointLightComponent());
			}
			ImGui::Separator();

			ImGui::EndMenu();

		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Add new system")) {
			if (ImGui::MenuItem("Scripting System")) {
				entityManager->addSystem(std::make_shared<ECS::ScriptingSystem>(), ECS::EngineStateTuple::APPLICATION);
			}
			ImGui::Separator();

			ImGui::EndMenu();

		}
		if(ImGui::MenuItem("Delete Entity")){
			entityManager->removeEntity(currentEntityID);
			//currentEntityID = 0;
			//entityManager->setCurrentEntity(currentEntityID);
		}

		
		ImGui::EndPopup();
	}

	if (deleted) {
		entityManager->removeEntity(currentEntityID);
		deleted = false;
	}

	ImGui::Begin("Entity positions");
	if(entityManager){
		auto ids = entityManager->getAssociatedEntities<ECS::TransformComponent>();
		for(int i = 0; i < ids.size(); i++){
			auto trans = entityManager->getComponent<ECS::TransformComponent>(ids[i]);
			std::string name = entityManager->getEntity(ids[i])->getName();
			ImGui::Text(name.c_str());
			trans->_display_gui_information_();
		}

	}
	ImGui::End();




	ImGui::End();
	
	/*Set tweaking stuff here*/
	
	//currentLayer->getShaderProgram()->setUniformVec1("u_brightness", brightness);


}

void vofog_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){

	Vofog::VofogCore* windowPointer = (Vofog::VofogCore*)glfwGetWindowUserPointer(window);
	Vofog::EventManager::getInstance().addMousePosition(xpos, ypos);
	windowPointer->onEvent();
}


void vofog_mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
	Vofog::VofogCore* windowPointer = (Vofog::VofogCore*)glfwGetWindowUserPointer(window);
	windowPointer->onEvent();
	if (Vofog::INPUT_STATE == 0) {

		static int mouseKey;
			
		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_1;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_2;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_3;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_4;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_5;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}
		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_6;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_7;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

		mouseKey = Vofog::VofogMouseKeys::VOFOG_MOUSE_BUTTON_8;
		if (button == mouseKey && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			Vofog::EventManager::getInstance().addMousePress(mouseKey);
		}
		if (button == mouseKey && action == GLFW_RELEASE) {
			Vofog::EventManager::getInstance().addMouseRelease(mouseKey);
		}

	}


}
void vofog_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	Vofog::VofogCore* windowPointer = (Vofog::VofogCore*)glfwGetWindowUserPointer(window);
	windowPointer->onEvent();
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]] = GLFW_KEY_BACKSPACE;
	else
		ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]] = 0;

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		ImGui::GetIO().KeysDown[GLFW_KEY_ENTER] = 1;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[GLFW_KEY_ENTER] = 0;
	}
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		ImGui::GetIO().KeysDown[GLFW_KEY_LEFT_CONTROL] = 1;
	}
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[GLFW_KEY_LEFT_CONTROL] = 0;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		ImGui::GetIO().KeysDown[GLFW_KEY_S] = 1;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[GLFW_KEY_S] = 0;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		ImGui::GetIO().KeysDown[GLFW_KEY_TAB] = 1;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[GLFW_KEY_TAB] = 0;
	}

	static auto keyCheck = GLFW_KEY_LEFT;

	keyCheck = GLFW_KEY_LEFT;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	keyCheck = GLFW_KEY_LEFT;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	keyCheck = GLFW_KEY_RIGHT;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	keyCheck = GLFW_KEY_UP;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	keyCheck = GLFW_KEY_DOWN;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	keyCheck = GLFW_KEY_DELETE;
	if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		ImGui::GetIO().KeysDown[keyCheck] = 1;
	}
	if (key == keyCheck && action == GLFW_RELEASE) {
		ImGui::GetIO().KeysDown[keyCheck] = 0;
	}

	if (Vofog::INPUT_STATE == 0) {
		//glfwSetWindowShouldClose(window, GL_TRUE);
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_SPACE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_APOSTROPHE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_COMMA; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_MINUS; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_PERIOD; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_SLASH; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_0; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_1; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_2; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_3; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_4; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_5; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_6; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_7; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_8; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_9; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_SEMICOLON; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_EQUAL; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_A; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_B; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_C; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_D; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_E; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_G; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_H; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_I; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_J; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_K; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_L; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_M; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_N; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_O; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_P; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_Q; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_R; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_S; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_T; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_U; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_V; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_W; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_X; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_Y; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_Z; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_BRACKET; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_BACKSLASH; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_BRACKET; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_GRAVE_ACCENT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_WORLD_1; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_WORLD_2; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_ESCAPE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_ENTER; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_TAB; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_BACKSPACE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_INSERT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_DELETE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_DOWN; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_UP; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_PAGE_UP; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_PAGE_DOWN; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_HOME; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_END; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_CAPS_LOCK; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_SCROLL_LOCK; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_NUM_LOCK; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_PRINT_SCREEN; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_PAUSE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F1; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F2; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F3; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F4; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F5; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F6; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F7; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F8; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F9; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F10; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F11; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F12; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F13; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F14; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F15; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F16; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F17; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F18; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F19; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F20; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F21; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F22; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F23; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F24; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_F25; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_0; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_1; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_2; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_3; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_4; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_5; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_6; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_7; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_8; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_9; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_DECIMAL; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_DIVIDE; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_MULTIPLY; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_SUBTRACT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_ADD; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_ENTER; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_KP_EQUAL; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_SHIFT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_CONTROL; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_ALT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_SUPER; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_SHIFT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_CONTROL; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_ALT; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_SUPER; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }
		keyCheck = Vofog::VofogKeyStore::VOFOG_KEY_MENU; if (key == keyCheck && (action == GLFW_PRESS || action == GLFW_REPEAT)) { Vofog::EventManager::getInstance().addKeyPress(keyCheck); }	if (key == keyCheck && action == GLFW_RELEASE) { Vofog::EventManager::getInstance().addKeyRelease(keyCheck); }


		//KK
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			if (windowPointer->activeMouse)glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			windowPointer->activeMouse = !windowPointer->activeMouse;
		}


	}
	



}

void Vofog::VofogCore::onDetach(){
	_sceneManger->onDetach();
	if(false){
		//std::ofstream oss("Project1.vofog", std::ios::out | std::ios::binary);
		std::ofstream oss(_projectFileName, std::ios::out | std::ios::binary);
		cereal::BinaryOutputArchive oarchive(oss);
		oarchive(_sceneManger);
		oss.close();

		std::ofstream myfile;
		myfile.open("recentProject.txt");
		myfile << _projectFileName.c_str();
		myfile.close();
	}
	if(true){
		//std::ofstream oss("Project1.vfg", std::ios::out | std::ios::binary);
		//std::ofstream oss(vfg_save_file(), std::ios::out | std::ios::binary);
		//cereal::BinaryOutputArchive oarchive(oss);
		//oarchive(_sceneManger);
	}
}


bool Vofog::VofogCore::windowClosed(){
	//VOFOG_ASSERT(_window != null);
	
	return glfwWindowShouldClose(_window);
}

