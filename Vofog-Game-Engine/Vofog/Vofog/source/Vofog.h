#pragma once
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define VFG_WIN
#ifdef _WIN64
	#define VFG_WIN_64
#else
	#define VFG_WIN_32
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
	#define VFG_LINUX
#elif __unix__ // all unices not caught above
	#define VFG_UNIX
#elif defined(_POSIX_VERSION)
	#define VFG_POSIX
#else
#   error "Unknown compiler"
#endif

typedef double vfg_time_unit;
extern vfg_time_unit Vofog_deltaTime;

#define VOFOG_RUNNER VofogCore
#define VOFOG_SHARE_GUI_DATA friend class VOFOG_RUNNER

/*typedef GLfloat VFGfloat;
typedef GLint VFGint;
typedef GLdouble VFGdouble;
typedef GLshort VFGshort;*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef VOFOG_LIBRARY
    #define VOFOG_API __declspec(dllexport)
#else 
    #define VOFOG_API __declspec(dllimport)

#endif


	
#ifdef VOFOG_DEBUG_BUILD
#define _VOFOG_DEBUG(x) x
#define VOFOG_ASSERT(expr) _VOFOG_DEBUG(assert(expr))
#define _VOFOG_DEBUG_BREAK_ DebugBreak()

#else
	#define _VOFOG_DEBUG(x) 
    #define VOFOG_ASSERT(expr) 
	#define _VOFOG_DEBUG_BREAK_
#endif

#define VOFOG_DEFUALT_RES_FILEPATH std::string(".//..//Vofog//resources//")


namespace Vofog{

	template <typename T>
	using Scope = std::unique_ptr<T>();

	template <typename T>
	using Ref = std::shared_ptr<T>();

}