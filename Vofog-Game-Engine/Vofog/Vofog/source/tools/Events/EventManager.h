#pragma once
#include <Vofog.h>
#include <vector>
#include <tools/Events/Event.h>
#include <tools/Events/EventKey.h>
#include <tools/Events/EventMouse.h>


namespace Vofog{
	
	/*struct VOFOG_API EventStorage{
		static std::vector<Vofog::EventKeyReleased> _keyReleasedEvents;
		static std::vector<Vofog::EventKeyPressed>  _keyPressedEvents;
	};*/

	//struct VOFOG_API MousePosition{
	//	double x = 0;
	//	double y = 0;

	//	MousePosition(){}
	//	MousePosition(double xx, double yy):x(xx), y(yy){}

	//};
	class VOFOG_API EventManager{

	private:
		static std::unique_ptr<EventManager> _instance;
		static std::vector<Vofog::VofogKeyValue> _keyReleasedEvents;
		static std::vector<Vofog::VofogKeyValue>  _keyPressedEvents;
		static std::vector<Vofog::VofogMouseKey>  _mousePressEvents;
		static std::vector<Vofog::VofogMouseKey>_mouseReleaseEvents;
		static std::vector<glm::vec2>  _mouseMovedEvents;
		static glm::vec2 _mousePosition;
		EventManager(){};
		static void create(){
			static EventManager instance;
			_instance = std::make_unique<EventManager>(instance);
		}
	public:
		//EventManager(const EventManager&) = delete;
		//EventManager operator=(const EventManager&) = delete;

		static EventManager& getInstance(){
			if(!_instance) create();
			return *_instance;
		}
		static void addKeyPress(Vofog::VofogKeyValue key){
			_keyPressedEvents.push_back(key);
		}
		static void addKeyRelease(Vofog::VofogKeyValue key){
			_keyReleasedEvents.push_back(key);
		}
		/*static void addEvent(const EventMouseMoved& event){
			_mouseMovedEvents.push_back(event);
		}*/
		static void addMousePosition(double x, double y){
			_mousePosition.x = x;
			_mousePosition.y = y;
			_mouseMovedEvents.push_back(glm::vec2(x, y));
		}
		static void addMousePress(VofogMouseKey keyPressed){
			_mousePressEvents.push_back(keyPressed);
		}
		static void addMouseRelease(VofogMouseKey keyReleased){
			_mouseReleaseEvents.push_back(keyReleased);
		}
		static void clear(){
			_keyPressedEvents.clear();
			_keyReleasedEvents.clear();
			_mouseMovedEvents.clear();
			_mousePressEvents.clear();
			_mouseReleaseEvents.clear();
		}

		inline static bool isKeyPressed(Vofog::VofogKeyValue key){
			for(int i = 0; i < _keyPressedEvents.size(); i++){
				if(_keyPressedEvents[i]== key)
					return true;
			}
			return false;
		}
		inline static bool isKeyReleased(Vofog::VofogKeyValue key){
			for(int i = 0; i < _keyReleasedEvents.size(); i++)
				if(_keyReleasedEvents[i] == key)
					return true;
				
			return false;
		}

		inline static bool isMousePressed(Vofog::VofogMouseKey key){
			for(int i = 0; i < _mousePressEvents.size(); i++){
				if(_mousePressEvents[i] == key)
					return true;
			}
			return false;
		}
		inline static bool isMouseReleased(Vofog::VofogMouseKey key){
			for(int i = 0; i < _mouseReleaseEvents.size(); i++){
				if(_mouseReleaseEvents[i] == key)
					return true;
			}
			return false;
		}

		inline static glm::vec2 getMouseMovedPosition(){
			return _mousePosition;
		}

	};
	

}