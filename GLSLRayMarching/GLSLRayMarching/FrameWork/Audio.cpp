//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Audio.h"

/////////////////////////////////////////////////////////////////////
Audio::Manager::Manager()
{
}

Audio::Manager::~Manager()
{
}

Audio::Manager& Audio::Manager::GetInstance()
{
	static Audio::Manager instance;

	return instance;
}

bool Audio::Manager::Initialize()
{
	return true;
}

bool Audio::Manager::Process()
{
	return true;
}

bool Audio::Manager::Pause()
{
	return true;
}

void Audio::Manager::Resume()
{
}

void Audio::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Audio::Service::Initialize()
{
	return Audio::Manager::GetInstance().Initialize();
}

bool Audio::Service::Process()
{
	return Audio::Manager::GetInstance().Process();
}

bool Audio::Service::Pause()
{
	return Audio::Manager::GetInstance().Pause();
}

void Audio::Service::Resume()
{
	Audio::Manager::GetInstance().Resume();
}

void Audio::Service::Terminate()
{
	Audio::Manager::GetInstance().Terminate();
}