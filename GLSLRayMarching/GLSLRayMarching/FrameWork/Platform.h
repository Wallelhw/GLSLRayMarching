//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Platform_h_
#define _Platform_h_

// include some basics Headers
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>

// include some std headers
#include <list>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
#include <functional>
#include <string>

#include <rttr/registration>

// platform define
//#define WINDOWS_32
#define WINDOWS_64
//#define MACOSX
//#define LINUX
//#define ANDROID
//#define IOS
//#define PS4
//#define XBOX
//#define NSWITCH

// Platform Encapsulation

#define Assert(cond)  assert((cond))
	//#define SCREEN_WIDTH 1080
	//#define SCREEN_HEIGHT 720
	//#define HALF_PI 1.570796327
	//#define ONE_PI 3.141592654
	//#define TWO_PI 6.283185308
	//#define DEG_2_RAD (ONE_PI / 180.0f)
	//#define RAD_2_DEG (180.0f / ONE_PI)

	enum class Modifier
	{
		SHIFT = 1,
		ALT = 2,
		CTRL = 4
	};

	class Platform
	{
	public:
		static void* window;

		static std::string appName;
		static int displayWidth;
		static int displayHeight;

		static int modifier;
		static int button;
		static int action;
		static float x;
		static float y;

		static int polygonLimit;

		static const char* Format(const char* format, ...);
		static void Verbose(const char* s, ...);
		static void Debug(const char* s, ...);
		static void Info(const char* s, ...);
		static void Warning(const char* s, ...);
		static void Error(const char* s, ...);

		static void MemSet(void* dst, int val, int size);
		static void MemCpy(void* dst, const void* src, int size);
		static int MemCmp(const void* s1, const void* s2, int size);
	};

#endif