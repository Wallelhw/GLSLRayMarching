//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Input_h_
#define _Input_h_

#include "Platform.h"

#include "String.h"

class Input
{
public:
	class Manager
	{
		Manager();
		~Manager();
	public:
		static Input::Manager& GetInstance();

		bool Initialize();
		bool Process();
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
		static bool Process();
		static bool Pause();
		static void Resume();
		static void Terminate();
	};
};

#endif