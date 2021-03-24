//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Physics2D_h_
#define _Physics2D_h_

#include "Platform.h"

#include "String.h"

class Physics2D
{
public:
	class Manager
	{
		Manager();
		~Manager();
	public:
		static Physics2D::Manager& GetInstance();

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