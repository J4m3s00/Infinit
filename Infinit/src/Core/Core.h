#pragma once

typedef std::string string;
typedef unsigned int uint;
typedef unsigned char byte;

#ifdef IN_PLATFORM_WINDOWS
	#ifdef IN_BUILD_DLL
		#define INFINIT_API __declspec(dllexport)
	#else
		#define INFINIT_API __declspec(dllimport)
	#endif
#endif