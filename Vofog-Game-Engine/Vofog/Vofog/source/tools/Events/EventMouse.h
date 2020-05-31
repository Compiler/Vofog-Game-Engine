#pragma once
#include <Vofog.h>
#include "Event.h"

namespace Vofog{

	class VOFOG_API EventMouseMoved: public Vofog::Event{


	private:
		double _xPosition, _yPosition;
		bool _mouseDragged;


	public:

		EventMouseMoved(double x, double y, bool dragged):
			Event(EventType::MouseMoved), _xPosition(x), _yPosition(y), _mouseDragged(dragged){
		}


		double getX() const { return _xPosition; }
		double getY() const { return _yPosition; }

		bool getMouseDragged(){ return _mouseDragged; }


#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("MouseMovedEvent"); }
#endif

		virtual ~EventMouseMoved(){}

	};

	class VOFOG_API EventMousePress: public Vofog::Event{

		private:
		Vofog::VofogMouseKey _button;

		public:

		EventMousePress(Vofog::VofogMouseKey mouseButton):
			Event(EventType::MouseButtonPressed), _button(mouseButton){}


		Vofog::VofogMouseKey getButton(){ return _button; }


		#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("MousePress"); }
		#endif

		virtual ~EventMousePress(){}
	};

	class VOFOG_API EventMouseRelease: public Vofog::Event{

	private:
		Vofog::VofogMouseKey _button;

	public:

		EventMouseRelease(Vofog::VofogMouseKey mouseButton):
			Event(EventType::MouseButtonReleased), _button(mouseButton){}

		Vofog::VofogMouseKey getButton(){ return _button; }


		#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("MouseRelease"); }
		#endif

		virtual ~EventMouseRelease(){}
	};



	class VOFOG_API EventMouseScrolled: public Vofog::Event{


	private:
		double _xOffset, _yOffset;
		bool _mouseDragged;


	public:

		EventMouseScrolled(double xoffset, double yoffset):
			Event(EventType::MouseScrolled), _xOffset(xoffset), _yOffset(yoffset){
		}


		double getXOffset() const{ return _xOffset; }
		double getYOffset() const{ return _yOffset; }



#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("MouseScrolledEvent"); }
#endif

		virtual ~EventMouseScrolled(){}

	};


}
