#pragma once

// Stupid windows DLL stuff (welcome to DLL hell)
#ifdef RT_BUILD_DLL
	#define RT_API __declspec( dllexport )
#else
	#define RT_API __declspec( dllimport )
#endif