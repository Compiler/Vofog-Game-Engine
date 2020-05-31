#pragma once
#include <Vofog.h>
#include "Event.h"

namespace Vofog{

	typedef int VofogKeyValue;

	namespace VofogKeyStore{
		constexpr VofogKeyValue VOFOG_KEY_SPACE			=	 32;
		constexpr VofogKeyValue VOFOG_KEY_APOSTROPHE    =	 39;  /* ' */
		constexpr VofogKeyValue VOFOG_KEY_COMMA         =	 44;  /* , */
		constexpr VofogKeyValue VOFOG_KEY_MINUS         =	 45;  /* - */
		constexpr VofogKeyValue VOFOG_KEY_PERIOD        =	 46;  /* . */
		constexpr VofogKeyValue VOFOG_KEY_SLASH         =	 47;  /* / */
		constexpr VofogKeyValue VOFOG_KEY_0             =	 48;
		constexpr VofogKeyValue VOFOG_KEY_1             =	 49;
		constexpr VofogKeyValue VOFOG_KEY_2             =	 50;
		constexpr VofogKeyValue VOFOG_KEY_3             =	 51;
		constexpr VofogKeyValue VOFOG_KEY_4             =	 52;
		constexpr VofogKeyValue VOFOG_KEY_5             =	 53;
		constexpr VofogKeyValue VOFOG_KEY_6             =	 54;
		constexpr VofogKeyValue VOFOG_KEY_7             =	 55;
		constexpr VofogKeyValue VOFOG_KEY_8             =	 56;
		constexpr VofogKeyValue VOFOG_KEY_9             =	 57;
		constexpr VofogKeyValue VOFOG_KEY_SEMICOLON     =	 59;  /* ; */
		constexpr VofogKeyValue VOFOG_KEY_EQUAL         =	 61;  /* = */
		constexpr VofogKeyValue VOFOG_KEY_A             =	 65;
		constexpr VofogKeyValue VOFOG_KEY_B             =	 66;
		constexpr VofogKeyValue VOFOG_KEY_C             =	 67;
		constexpr VofogKeyValue VOFOG_KEY_D             =	 68;
		constexpr VofogKeyValue VOFOG_KEY_E             =	 69;
		constexpr VofogKeyValue VOFOG_KEY_F             =	 70;
		constexpr VofogKeyValue VOFOG_KEY_G             =	 71;
		constexpr VofogKeyValue VOFOG_KEY_H             =	 72;
		constexpr VofogKeyValue VOFOG_KEY_I             =	 73;
		constexpr VofogKeyValue VOFOG_KEY_J             =	 74;
		constexpr VofogKeyValue VOFOG_KEY_K             =	 75;
		constexpr VofogKeyValue VOFOG_KEY_L             =	 76;
		constexpr VofogKeyValue VOFOG_KEY_M             =	 77;
		constexpr VofogKeyValue VOFOG_KEY_N             =	 78;
		constexpr VofogKeyValue VOFOG_KEY_O             =	 79;
		constexpr VofogKeyValue VOFOG_KEY_P             =	 80;
		constexpr VofogKeyValue VOFOG_KEY_Q             =	 81;
		constexpr VofogKeyValue VOFOG_KEY_R             =	 82;
		constexpr VofogKeyValue VOFOG_KEY_S             =	 83;
		constexpr VofogKeyValue VOFOG_KEY_T             =	 84;
		constexpr VofogKeyValue VOFOG_KEY_U             =	 85;
		constexpr VofogKeyValue VOFOG_KEY_V             =	 86;
		constexpr VofogKeyValue VOFOG_KEY_W             =	 87;
		constexpr VofogKeyValue VOFOG_KEY_X             =	 88;
		constexpr VofogKeyValue VOFOG_KEY_Y             =	 89;
		constexpr VofogKeyValue VOFOG_KEY_Z             =	 90;
		constexpr VofogKeyValue VOFOG_KEY_LEFT_BRACKET  =	 91;  /* [ */
		constexpr VofogKeyValue VOFOG_KEY_BACKSLASH     =	 92;  /* \ */
		constexpr VofogKeyValue VOFOG_KEY_RIGHT_BRACKET =	 93;  /* ] */
		constexpr VofogKeyValue VOFOG_KEY_GRAVE_ACCENT  =	 96;  /* ` */
		constexpr VofogKeyValue VOFOG_KEY_WORLD_1       =	 161; /* non-US #1 */
		constexpr VofogKeyValue VOFOG_KEY_WORLD_2       =	 162; /* non-US #2 */
		constexpr VofogKeyValue VOFOG_KEY_ESCAPE        =	 256;
		constexpr VofogKeyValue VOFOG_KEY_ENTER         =	 257;
		constexpr VofogKeyValue VOFOG_KEY_TAB           =	 258;
		constexpr VofogKeyValue VOFOG_KEY_BACKSPACE     =	 259;
		constexpr VofogKeyValue VOFOG_KEY_INSERT        =	 260;
		constexpr VofogKeyValue VOFOG_KEY_DELETE        =	 261;
		constexpr VofogKeyValue VOFOG_KEY_RIGHT         =	 262;
		constexpr VofogKeyValue VOFOG_KEY_LEFT          =	 263;
		constexpr VofogKeyValue VOFOG_KEY_DOWN          =	 264;
		constexpr VofogKeyValue VOFOG_KEY_UP            =	 265;
		constexpr VofogKeyValue VOFOG_KEY_PAGE_UP       =	 266;
		constexpr VofogKeyValue VOFOG_KEY_PAGE_DOWN     =	 267;
		constexpr VofogKeyValue VOFOG_KEY_HOME          =	 268;
		constexpr VofogKeyValue VOFOG_KEY_END           =	 269;
		constexpr VofogKeyValue VOFOG_KEY_CAPS_LOCK     =	 280;
		constexpr VofogKeyValue VOFOG_KEY_SCROLL_LOCK   =	 281;
		constexpr VofogKeyValue VOFOG_KEY_NUM_LOCK      =	 282;
		constexpr VofogKeyValue VOFOG_KEY_PRINT_SCREEN  =	 283;
		constexpr VofogKeyValue VOFOG_KEY_PAUSE         =	 284;
		constexpr VofogKeyValue VOFOG_KEY_F1            =	 290;
		constexpr VofogKeyValue VOFOG_KEY_F2            =	 291;
		constexpr VofogKeyValue VOFOG_KEY_F3            =	 292;
		constexpr VofogKeyValue VOFOG_KEY_F4            =	 293;
		constexpr VofogKeyValue VOFOG_KEY_F5            =	 294;
		constexpr VofogKeyValue VOFOG_KEY_F6            =	 295;
		constexpr VofogKeyValue VOFOG_KEY_F7            =	 296;
		constexpr VofogKeyValue VOFOG_KEY_F8            =	 297;
		constexpr VofogKeyValue VOFOG_KEY_F9            =	 298;
		constexpr VofogKeyValue VOFOG_KEY_F10           =	 299;
		constexpr VofogKeyValue VOFOG_KEY_F11           =	 300;
		constexpr VofogKeyValue VOFOG_KEY_F12           =	 301;
		constexpr VofogKeyValue VOFOG_KEY_F13           =	 302;
		constexpr VofogKeyValue VOFOG_KEY_F14           =	 303;
		constexpr VofogKeyValue VOFOG_KEY_F15           =	 304;
		constexpr VofogKeyValue VOFOG_KEY_F16           =	 305;
		constexpr VofogKeyValue VOFOG_KEY_F17           =	 306;
		constexpr VofogKeyValue VOFOG_KEY_F18           =	 307;
		constexpr VofogKeyValue VOFOG_KEY_F19           =	 308;
		constexpr VofogKeyValue VOFOG_KEY_F20           =	 309;
		constexpr VofogKeyValue VOFOG_KEY_F21           =	 310;
		constexpr VofogKeyValue VOFOG_KEY_F22           =	 311;
		constexpr VofogKeyValue VOFOG_KEY_F23           =	 312;
		constexpr VofogKeyValue VOFOG_KEY_F24           =	 313;
		constexpr VofogKeyValue VOFOG_KEY_F25           =	 314;
		constexpr VofogKeyValue VOFOG_KEY_KP_0          =	 320;
		constexpr VofogKeyValue VOFOG_KEY_KP_1          =	 321;
		constexpr VofogKeyValue VOFOG_KEY_KP_2          =	 322;
		constexpr VofogKeyValue VOFOG_KEY_KP_3          =	 323;
		constexpr VofogKeyValue VOFOG_KEY_KP_4          =	 324;
		constexpr VofogKeyValue VOFOG_KEY_KP_5          =	 325;
		constexpr VofogKeyValue VOFOG_KEY_KP_6          =	 326;
		constexpr VofogKeyValue VOFOG_KEY_KP_7          =	 327;
		constexpr VofogKeyValue VOFOG_KEY_KP_8          =	 328;
		constexpr VofogKeyValue VOFOG_KEY_KP_9          =	 329;
		constexpr VofogKeyValue VOFOG_KEY_KP_DECIMAL    =	 330;
		constexpr VofogKeyValue VOFOG_KEY_KP_DIVIDE     =	 331;
		constexpr VofogKeyValue VOFOG_KEY_KP_MULTIPLY   =	 332;
		constexpr VofogKeyValue VOFOG_KEY_KP_SUBTRACT   =	 333;
		constexpr VofogKeyValue VOFOG_KEY_KP_ADD        =	 334;
		constexpr VofogKeyValue VOFOG_KEY_KP_ENTER      =	 335;
		constexpr VofogKeyValue VOFOG_KEY_KP_EQUAL      =	 336;
		constexpr VofogKeyValue VOFOG_KEY_LEFT_SHIFT    =	 340;
		constexpr VofogKeyValue VOFOG_KEY_LEFT_CONTROL  =	 341;
		constexpr VofogKeyValue VOFOG_KEY_LEFT_ALT      =	 342;
		constexpr VofogKeyValue VOFOG_KEY_LEFT_SUPER    =	 343;
		constexpr VofogKeyValue VOFOG_KEY_RIGHT_SHIFT   =	 344;
		constexpr VofogKeyValue VOFOG_KEY_RIGHT_CONTROL =	 345;
		constexpr VofogKeyValue VOFOG_KEY_RIGHT_ALT     =	 346;
		constexpr VofogKeyValue VOFOG_KEY_RIGHT_SUPER   =	 347;
		constexpr VofogKeyValue VOFOG_KEY_MENU          =	 348;
	}
	class VOFOG_API EventKeyPressed: public Vofog::Event{


	private:
		
		VofogKeyValue _key;


	public:

		EventKeyPressed(VofogKeyValue key):
			Event(EventType::KeyPressed), _key(key){
		}


		VofogKeyValue getKey() const{ return _key; }


#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("MouseKeyPressed"); }
#endif

		virtual ~EventKeyPressed(){}

	};


	class VOFOG_API EventKeyReleased: public Vofog::Event{


	private:

		VofogKeyValue _key;


	public:

		EventKeyReleased(VofogKeyValue key):
			Event(EventType::KeyReleased), _key(key){
		}


		VofogKeyValue getKey() const{ return _key; }


#ifdef VOFOG_DEBUG_BUILD
		void VOFOG_DEBUG_printName() const override{ VOFOG_CORE_DEBUG("EventKeyReleased"); }
#endif

		virtual ~EventKeyReleased(){}

	};
}