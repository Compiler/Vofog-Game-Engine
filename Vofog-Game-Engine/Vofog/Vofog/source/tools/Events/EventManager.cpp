#include <vpch.h>
#include "EventManager.h"

namespace Vofog{


	std::unique_ptr<EventManager> EventManager::_instance;
	std::vector<Vofog::VofogKeyValue>  EventManager::_keyPressedEvents;
	std::vector<Vofog::VofogKeyValue> EventManager::_keyReleasedEvents;

	glm::vec2 EventManager::_mousePosition;
	std::vector<glm::vec2>  EventManager::_mouseMovedEvents;
	std::vector<Vofog::VofogMouseKey>  EventManager::_mousePressEvents;
	std::vector<Vofog::VofogMouseKey>  EventManager::_mouseReleaseEvents;

}