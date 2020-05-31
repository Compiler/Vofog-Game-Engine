#pragma once


#include <Vofog.h>


namespace Vofog{
	typedef int VofogMouseKey;
	namespace VofogMouseKeys{
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_1 = 0;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_2 = 1;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_3 = 2;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_4 = 3;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_5 = 4;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_6 = 5;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_7 = 6;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_8 = 7;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_LEFT = VOFOG_MOUSE_BUTTON_1;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_RIGHT = VOFOG_MOUSE_BUTTON_2;
		constexpr VofogMouseKey VOFOG_MOUSE_BUTTON_MIDDLE = VOFOG_MOUSE_BUTTON_3;

	}
	class VOFOG_API Event{
		
	public:

		enum class EventType{
			None = 0,
			MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
			WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
			KeyPressed, KeyReleased, MouseButton_Left, MouseButton_Right

		};


#ifdef VOFOG_DEBUG_BUILD
		virtual void VOFOG_DEBUG_printName() const { VOFOG_CORE_DEBUG("Event has no type.");  }
#endif


		void handled(){ _eventHandled = true; }
		EventType getType() const { return _eventType; }

		inline bool isHandled() const{ return _eventHandled; }

		virtual ~Event(){};
	private:

		EventType _eventType;

		bool _eventHandled = false;


	protected:

		Event(EventType type): _eventType(type){

		
		}




	};


}