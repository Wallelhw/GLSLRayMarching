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

// Platform Encapsulation
#define PLATFORM WINDOWS_64
//#define PLATFORM MACOSX
//#define PLATFORM LINUX
//#define PLATFORM ANDROID
//#define PLATFORM IOS
//#define PLATFORM PS4
//#define PLATFORM XBOXONE
//#define PLATFORM NSWITCH
//#define PLATFORM PS5
//#define PLATFORM XSX

enum class Modifier
{
	SHIFT = 1,
	ALT = 2,
	CTRL = 4
};

class Platform
{
public:
#define KEY_COUNT 256

#define JOYSTICK_COUNT 16
#define JOYSTICK_AXE_COUNT 4
#define JOYSTICK_BUTTON_COUNT 16
	class JoyStick
	{
	public:
		float axis[JOYSTICK_AXE_COUNT];
		bool button[JOYSTICK_BUTTON_COUNT];
	};

	class Mouse
	{
	public:
		float x;
		float y;
		int button;
		float dx;
		float dy;
	};
	static bool Instantiate(int width_, int height_, const char* appName_, const char* InitalizeScene_);
	static bool PreUpdate();
	static bool Update();
	static bool PostUpdate();
	static bool Pause();
	static void Resume();
	static void Terminate();

	static void* GetHandle();
	static const std::string& GetAppName();
	static const std::string& GetInitialScene();
	static int GetWidth();
	static int GetHeight();

	static int GetKeyCount();
	static bool GetKey(int i);
	
	static int GetJoyStickCount();
	static JoyStick GetJoyStick(int i);

	static Mouse GetMouse();
	static bool GetMouseButton(int which);
	static float GetMouseX();
	static float GetMouseY();
	static float GetMouseDX();
	static float GetMouseDY();


	static double GetTime();
	static double GetDeltaTime();
	static int GetTotalFrameCounter();
	static int GetSceneFrameCounter();
	static void ResetSceneFrameCounter();

	static bool QuitApp();
private:
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

#define Assert(cond)  assert((cond))

#endif