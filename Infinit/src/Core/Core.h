#pragma once


#ifdef IN_PLATFORM_WINDOWS
	#ifdef IN_BUILD_DLL
		#define INFINIT_API __declspec(dllexport)
	#else
		#define INFINIT_API __declspec(dllimport)
	#endif
#endif