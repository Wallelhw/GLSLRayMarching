//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include <time.h>       /* time_t, struct tm, time, localtime */

time_t startTimer;
time_t lastTimer;

double Platform::time = 0;
double Platform::elapsedTime = 0;

bool Platform::Instantiate()
{
	::time(&startTimer);

	return true;
}

bool Platform::Update()
{
	time_t timer;
	::time(&timer);

	time = difftime(timer, startTimer);
	elapsedTime = difftime(timer, lastTimer);

	lastTimer = timer;

	return true;
}

bool Platform::Pause()
{
	return true;
}

void Platform::Resume()
{
}

void Platform::Terminate()
{
}

double Platform::GetTime()
{
	return time;
}

double Platform::GetElapsed()
{
	return elapsedTime;
}

const char* Format(const char* format, ...)
{
	static char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	return (const char*)buffer;
}

void Verbose(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Verbose: %s", buffer);
}

void Debug(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Debug: %s", buffer);
}

void Info(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Info: %s", buffer);
}

void Warning(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Warning: %s", buffer);
}

void Error(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Error: %s", buffer);
}

void MemSet(void* dst, int val, int size)
{
	::memset(dst, val, size);
}

void MemCpy(void* dst, const void* src, int size)
{
	::memcpy(dst, src, size);
}

int MemCmp(const void* s1, const void* s2, int size)
{
	return ::memcmp(s1, s2, size);
}