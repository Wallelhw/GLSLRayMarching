//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"

#define Assert(cond)  assert((cond))

	void* Platform::window = nullptr;

	std::string Platform::appName = "Default";
	int Platform::displayWidth = 1920;
	int Platform::displayHeight = 1080;

	int Platform::modifier = 0;
	int Platform::button = 0;
	int Platform::action = 0;
	float Platform::x = 0;
	float Platform::y = 0;

	int Platform::polygonLimit = 300;

	const char* Platform::Format(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		return(buffer);
	}

	void Platform::Verbose(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		printf("Verbose: %s", buffer);
	}

	void Platform::Debug(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		printf("Debug: %s", buffer);
	}

	void Platform::Info(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		printf("Info: %s", buffer);
	}

	void Platform::Warning(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		printf("Warning: %s", buffer);
	}

	void Platform::Error(const char* format, ...)
	{
		char buffer[4096];

		va_list aptr;
		int ret;

		va_start(aptr, format);
		ret = vsprintf(buffer, format, aptr);
		va_end(aptr);

		printf("Error: %s", buffer);
	}

	void Platform::MemSet(void* dst, int val, int size)
	{
		::memset(dst, val, size);
	}

	void Platform::MemCpy(void* dst, const void* src, int size)
	{
		::memcpy(dst, src, size);
	}

	int Platform::MemCmp(const void* s1, const void* s2, int size)
	{
		return ::memcmp(s1, s2, size);
	}
