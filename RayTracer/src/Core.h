#pragma once

#ifdef RT_BUILD_DLL
	#define RT_API _declspec(dllexport)
#else
	#define RT_API _declspec(dllimport)
#endif