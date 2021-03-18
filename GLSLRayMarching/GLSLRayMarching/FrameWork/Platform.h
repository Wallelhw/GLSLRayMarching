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

enum class Modifier
{
	SHIFT = 1,
	ALT = 2,
	CTRL = 4
};

class Platform
{
public:
	static bool Instantiate();
	static bool Update();
	static bool Pause();
	static void Resume();
	static void Terminate();

	static double GetTime();
	static double GetElapsed();
private:
	static double time;
	static double elapsedTime;
};

void MemSet(void* dst, int val, int size);
void MemCpy(void* dst, const void* src, int size);
int MemCmp(const void* s1, const void* s2, int size);

const char* Format(const char* format, ...);
void Verbose(const char* s, ...);
void Debug(const char* s, ...);
void Info(const char* s, ...);
void Warning(const char* s, ...);
void Error(const char* s, ...);

//static 
//time_t time(time_t* timer);
//
#endif