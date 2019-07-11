#pragma once

typedef std::string string;
typedef unsigned int uint;
typedef unsigned char byte;

#define IN_ENABLE_ASSERT

#if 0
	#ifdef IN_PLATFORM_WINDOWS
		#ifdef IN_BUILD_DLL
			#define INFINIT_API __declspec(dllexport)
		#else
			#define INFINIT_API __declspec(dllimport)
		#endif
	#endif
#else 
	#define INFINIT_API
#endif

#ifdef IN_DEBUG
	#define IN_ENABLE_ASSERT
#endif

#ifdef IN_ENABLE_ASSERT
#define IN_ASSERT(x, ...) { if(!(x)) { IN_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
#define IN_CORE_ASSERT(x, ...) { if(!(x)) { IN_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define IN_ASSERT(x, ...)
	#define IN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define IN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define M_PI 3.1415f
#define TO_RADIANS (M_PI / 180.0f)
#define TO_DEGREES (180.0f / M_PI)