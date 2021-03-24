//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Audio_h_
#define _Audio_h_

#include "Platform.h"

#include "String.h"

class Audio
{
public:
	class Manager
	{
	friend class Audio;
		Manager();
		~Manager();
	public:
		static Audio::Manager& GetInstance();
	private:
		bool Initialize();
		bool Update();
		bool Pause();
		void Resume();
		void Terminate();
	};

	///////////////////////////////////////////////////////////////////////
public:
	class Service
	{
	public:
		static bool Initialize();
		static bool Update();
		static bool Pause();
		static void Resume();
		static void Terminate();
	};
};

#endif