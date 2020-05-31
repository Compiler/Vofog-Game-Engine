#include <vpch.h>
#include "LuaState.h"

sol::state* Vofog::LuaState::lua = new sol::state();

void Vofog::LuaState::initialize() {
	LuaState::lua->open_libraries(sol::lib::base);


	sol::usertype<glm::vec2> vec2_type = LuaState::lua->new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2()>());
	vec2_type["x"] = &glm::vec2::x; 
	vec2_type["y"] = &glm::vec2::y; 

	sol::usertype<glm::vec3> vec3_type = LuaState::lua->new_usertype<glm::vec3>("vec3", sol::constructors<glm::vec3()>());
	vec3_type["x"] = &glm::vec3::x; vec3_type["y"] = &glm::vec3::y; vec3_type["z"] = &glm::vec3::z;

	sol::usertype<btVector3> btvec3_type = LuaState::lua->new_usertype<btVector3>("btvec3", sol::constructors<btVector3(), btVector3(float, float, float)>());
	btvec3_type["x"] = &btVector3::x; btvec3_type["y"] = &btVector3::y; btvec3_type["z"] = &btVector3::z;

	sol::usertype<Vofog::ECS::TransformComponent> transform_type = LuaState::lua->new_usertype<Vofog::ECS::TransformComponent>("TransformComponent", sol::constructors<Vofog::ECS::TransformComponent()>());
	transform_type["position"] =	&Vofog::ECS::TransformComponent::position;
	transform_type["rotation"] =	&Vofog::ECS::TransformComponent::rotation;
	transform_type["size"] =		&Vofog::ECS::TransformComponent::size;

	sol::usertype<Vofog::ECS::PhysicsComponent> physics_type = LuaState::lua->new_usertype<Vofog::ECS::PhysicsComponent>("PhysicsComponent", sol::constructors<Vofog::ECS::PhysicsComponent()>());
	physics_type["body"] = &Vofog::ECS::PhysicsComponent::body;

	
	sol::usertype<btRigidBody> body_type = LuaState::lua->new_usertype<btRigidBody>("RigidBody");
	body_type["applyCentralForce"] = &btRigidBody::applyCentralForce;
	body_type["applyCentralImpulse"] = &btRigidBody::applyCentralImpulse;
	body_type["setState"] = &btRigidBody::setActivationState;
	//VOFOG_CORE_LOG(e1.toString());
	//VOFOG_CORE_LOG(e2.toString());
	//VOFOG_CORE_LOG(e3.toString());
	//VOFOG_CORE_LOG(e4.toString());
	sol::usertype<ECS::Entity> entity_type = LuaState::lua->new_usertype<ECS::Entity>("Entity", sol::constructors<ECS::Entity(), ECS::Entity(const char*)>());
	entity_type["getID"] = &ECS::Entity::getID;
	entity_type["getName"] = &ECS::Entity::getName;
	entity_type["setName"] = &ECS::Entity::setName;
	entity_type["toString"] = &ECS::Entity::toString;
	//entity_type["getComponent"] = 


	



	sol::usertype<Vofog::ECS::EntityManager> ecs_type = LuaState::lua->new_usertype<Vofog::ECS::EntityManager>("EntityManager");
	ecs_type["getCurrentEntity"] = &Vofog::ECS::EntityManager::getCurrentEntity;
	ecs_type["getEntityByName"] = &Vofog::ECS::EntityManager::getEntityByName;
	ecs_type["getTransform"] = &Vofog::ECS::EntityManager::getTransformComponent;
	ecs_type["getPhysics"] = &Vofog::ECS::EntityManager::getPhysicsComponent;
	ecs_type["addEntity"] = &Vofog::ECS::EntityManager::_LUA_addEntity;
	ecs_type["addRenderableEntity"] = &Vofog::ECS::EntityManager::_LUA_addRenderableEntity;

	sol::usertype<Vofog::EventManager> input_type = LuaState::lua->new_usertype<Vofog::EventManager>("Input");
	input_type["isMouseButtonDown"] = &Vofog::EventManager::isMousePressed;
	input_type["isKeyDown"] = &Vofog::EventManager::isKeyPressed;
	input_type["mousePosition"] = &Vofog::EventManager::getMouseMovedPosition;


	(*Vofog::LuaState::lua).set_function("vprint", [](std::string arg) {
		VOFOG_CORE_TRACE("[LUA]: {}", arg);
		});
	//(*Vofog::LuaState::lua)["vprint"] = &::Vofog::Logging::getCoreLogger()->trace;
	char nextChar = 'A';
	for (int i = 0; i < 26; i++) {
		std::string key = "Key";key.push_back(nextChar);(*Vofog::LuaState::lua)[key.c_str()] = (int)(nextChar++);
	}
	nextChar = '0';
	for (int i = 0; i < 10; i++) {
		std::string key = "Key"; key.push_back(nextChar); (*Vofog::LuaState::lua)[key.c_str()] = (int)(nextChar++); 
	}
	(*Vofog::LuaState::lua)["KeySpace"] = Vofog::VofogKeyStore::VOFOG_KEY_SPACE;
	(*Vofog::LuaState::lua)["KeyApostrophe"] = Vofog::VofogKeyStore::VOFOG_KEY_APOSTROPHE;
	(*Vofog::LuaState::lua)["KeyComma"] = Vofog::VofogKeyStore::VOFOG_KEY_COMMA;
	(*Vofog::LuaState::lua)["KeyMinus"] = Vofog::VofogKeyStore::VOFOG_KEY_MINUS;
	(*Vofog::LuaState::lua)["KeyPeriod"] = Vofog::VofogKeyStore::VOFOG_KEY_PERIOD;
	(*Vofog::LuaState::lua)["KeySlash"] = Vofog::VofogKeyStore::VOFOG_KEY_SLASH;
	(*Vofog::LuaState::lua)["KeySemicolon"] = Vofog::VofogKeyStore::VOFOG_KEY_SEMICOLON;
	(*Vofog::LuaState::lua)["KeyEqual"] = Vofog::VofogKeyStore::VOFOG_KEY_EQUAL;
	(*Vofog::LuaState::lua)["KeyLeftBracket"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_BRACKET;
	(*Vofog::LuaState::lua)["KeyBackSlash"] = Vofog::VofogKeyStore::VOFOG_KEY_BACKSLASH;
	(*Vofog::LuaState::lua)["KeyRightBracket"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_BRACKET;
	(*Vofog::LuaState::lua)["KeyGraveAccent"] = Vofog::VofogKeyStore::VOFOG_KEY_GRAVE_ACCENT;
	(*Vofog::LuaState::lua)["KeyWorld1"] = Vofog::VofogKeyStore::VOFOG_KEY_WORLD_1;
	(*Vofog::LuaState::lua)["KeyWorld2"] = Vofog::VofogKeyStore::VOFOG_KEY_WORLD_2;
	(*Vofog::LuaState::lua)["KeyEscape"] = Vofog::VofogKeyStore::VOFOG_KEY_ESCAPE;
	(*Vofog::LuaState::lua)["KeyEnter"] = Vofog::VofogKeyStore::VOFOG_KEY_ENTER;
	(*Vofog::LuaState::lua)["KeyTab"] = Vofog::VofogKeyStore::VOFOG_KEY_TAB;
	(*Vofog::LuaState::lua)["KeyBackSpace"] = Vofog::VofogKeyStore::VOFOG_KEY_BACKSPACE;
	(*Vofog::LuaState::lua)["KeyInsert"] = Vofog::VofogKeyStore::VOFOG_KEY_INSERT;
	(*Vofog::LuaState::lua)["KeyDelete"] = Vofog::VofogKeyStore::VOFOG_KEY_DELETE;
	(*Vofog::LuaState::lua)["KeyRight"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT;
	(*Vofog::LuaState::lua)["KeyLeft"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT;
	(*Vofog::LuaState::lua)["KeyDown"] = Vofog::VofogKeyStore::VOFOG_KEY_DOWN;
	(*Vofog::LuaState::lua)["KeyUp"] = Vofog::VofogKeyStore::VOFOG_KEY_UP;
	(*Vofog::LuaState::lua)["KeyPageUp"] = Vofog::VofogKeyStore::VOFOG_KEY_PAGE_UP;
	(*Vofog::LuaState::lua)["KeyPageDown"] = Vofog::VofogKeyStore::VOFOG_KEY_PAGE_DOWN;
	(*Vofog::LuaState::lua)["KeyHome"] = Vofog::VofogKeyStore::VOFOG_KEY_HOME;
	(*Vofog::LuaState::lua)["KeyEnd"] = Vofog::VofogKeyStore::VOFOG_KEY_END;
	(*Vofog::LuaState::lua)["KeyCapsLock"] = Vofog::VofogKeyStore::VOFOG_KEY_CAPS_LOCK;
	(*Vofog::LuaState::lua)["KeyScrollLock"] = Vofog::VofogKeyStore::VOFOG_KEY_SCROLL_LOCK;
	(*Vofog::LuaState::lua)["KeyNumLock"] = Vofog::VofogKeyStore::VOFOG_KEY_NUM_LOCK;
	(*Vofog::LuaState::lua)["KeyPrintScreen"] = Vofog::VofogKeyStore::VOFOG_KEY_PRINT_SCREEN;
	(*Vofog::LuaState::lua)["KeyKeyPause"] = Vofog::VofogKeyStore::VOFOG_KEY_PAUSE;
	(*Vofog::LuaState::lua)["KeyF1"] = Vofog::VofogKeyStore::VOFOG_KEY_F1;
	(*Vofog::LuaState::lua)["KeyF2"] = Vofog::VofogKeyStore::VOFOG_KEY_F2;
	(*Vofog::LuaState::lua)["KeyF3"] = Vofog::VofogKeyStore::VOFOG_KEY_F3;
	(*Vofog::LuaState::lua)["KeyF4"] = Vofog::VofogKeyStore::VOFOG_KEY_F4;
	(*Vofog::LuaState::lua)["KeyF5"] = Vofog::VofogKeyStore::VOFOG_KEY_F5;
	(*Vofog::LuaState::lua)["KeyF6"] = Vofog::VofogKeyStore::VOFOG_KEY_F6;
	(*Vofog::LuaState::lua)["KeyF7"] = Vofog::VofogKeyStore::VOFOG_KEY_F7;
	(*Vofog::LuaState::lua)["KeyF8"] = Vofog::VofogKeyStore::VOFOG_KEY_F8;
	(*Vofog::LuaState::lua)["KeyF9"] = Vofog::VofogKeyStore::VOFOG_KEY_F9;
	(*Vofog::LuaState::lua)["KeyF10"] = Vofog::VofogKeyStore::VOFOG_KEY_F10;
	(*Vofog::LuaState::lua)["KeyF11"] = Vofog::VofogKeyStore::VOFOG_KEY_F11;
	(*Vofog::LuaState::lua)["KeyF12"] = Vofog::VofogKeyStore::VOFOG_KEY_F12;
	(*Vofog::LuaState::lua)["KeyF13"] = Vofog::VofogKeyStore::VOFOG_KEY_F13;
	(*Vofog::LuaState::lua)["KeyF14"] = Vofog::VofogKeyStore::VOFOG_KEY_F14;
	(*Vofog::LuaState::lua)["KeyF15"] = Vofog::VofogKeyStore::VOFOG_KEY_F15;
	(*Vofog::LuaState::lua)["KeyF16"] = Vofog::VofogKeyStore::VOFOG_KEY_F16;
	(*Vofog::LuaState::lua)["KeyF17"] = Vofog::VofogKeyStore::VOFOG_KEY_F17;
	(*Vofog::LuaState::lua)["KeyF18"] = Vofog::VofogKeyStore::VOFOG_KEY_F18;
	(*Vofog::LuaState::lua)["KeyF19"] = Vofog::VofogKeyStore::VOFOG_KEY_F19;
	(*Vofog::LuaState::lua)["KeyF20"] = Vofog::VofogKeyStore::VOFOG_KEY_F20;
	(*Vofog::LuaState::lua)["KeyF21"] = Vofog::VofogKeyStore::VOFOG_KEY_F21;
	(*Vofog::LuaState::lua)["KeyF22"] = Vofog::VofogKeyStore::VOFOG_KEY_F22;
	(*Vofog::LuaState::lua)["KeyF23"] = Vofog::VofogKeyStore::VOFOG_KEY_F23;
	(*Vofog::LuaState::lua)["KeyF24"] = Vofog::VofogKeyStore::VOFOG_KEY_F24;
	(*Vofog::LuaState::lua)["KeyF25"] = Vofog::VofogKeyStore::VOFOG_KEY_F25;
	(*Vofog::LuaState::lua)["KeyKeyPad0"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_0;
	(*Vofog::LuaState::lua)["KeyKeyPad1"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_1;
	(*Vofog::LuaState::lua)["KeyKeyPad2"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_2;
	(*Vofog::LuaState::lua)["KeyKeyPad3"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_3;
	(*Vofog::LuaState::lua)["KeyKeyPad4"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_4;
	(*Vofog::LuaState::lua)["KeyKeyPad5"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_5;
	(*Vofog::LuaState::lua)["KeyKeyPad6"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_6;
	(*Vofog::LuaState::lua)["KeyKeyPad7"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_7;
	(*Vofog::LuaState::lua)["KeyKeyPad8"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_8;
	(*Vofog::LuaState::lua)["KeyKeyPad9"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_9;
	(*Vofog::LuaState::lua)["KeyKeyPadDecimal"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_DECIMAL;
	(*Vofog::LuaState::lua)["KeyKeyPadDivide"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_DIVIDE;
	(*Vofog::LuaState::lua)["KeyKeyPadMultiply"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_MULTIPLY;
	(*Vofog::LuaState::lua)["KeyKeyPadSubtract"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_SUBTRACT;
	(*Vofog::LuaState::lua)["KeyKeyPadAdd"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_ADD;
	(*Vofog::LuaState::lua)["KeyKeyPadEnter"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_ENTER;
	(*Vofog::LuaState::lua)["KeyKeyPadEqual"] = Vofog::VofogKeyStore::VOFOG_KEY_KP_EQUAL;
	(*Vofog::LuaState::lua)["KeyLeftShift"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_SHIFT;
	(*Vofog::LuaState::lua)["KeyLeftControl"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_CONTROL;
	(*Vofog::LuaState::lua)["KeyLeftAlt"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_ALT;
	(*Vofog::LuaState::lua)["KeyLeftSuper"] = Vofog::VofogKeyStore::VOFOG_KEY_LEFT_SUPER;
	(*Vofog::LuaState::lua)["KeyRightShift"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_SHIFT;
	(*Vofog::LuaState::lua)["KeyRightControl"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_CONTROL;
	(*Vofog::LuaState::lua)["KeyRightAlt"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_ALT;
	(*Vofog::LuaState::lua)["KeyRightSuper"] = Vofog::VofogKeyStore::VOFOG_KEY_RIGHT_SUPER;
	(*Vofog::LuaState::lua)["KeyMenu"] = Vofog::VofogKeyStore::VOFOG_KEY_MENU;

}