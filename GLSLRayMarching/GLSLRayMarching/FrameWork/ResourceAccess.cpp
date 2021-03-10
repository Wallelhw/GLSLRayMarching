//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "ResourceAccess.h"

/////////////////////////////////////////////////////////////////////
ResourceAccess::Manager::Manager()
{
}

ResourceAccess::Manager::~Manager()
{
}

ResourceAccess::Manager& ResourceAccess::Manager::GetInstance()
{
	static ResourceAccess::Manager instance;

	return instance;
}

bool ResourceAccess::Manager::Initialize()
{
	return true;
}

bool ResourceAccess::Manager::Process()
{
	return true;
}

bool ResourceAccess::Manager::Pause()
{
	return true;
}

void ResourceAccess::Manager::Resume()
{
}

void ResourceAccess::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool ResourceAccess::Service::Initialize()
{
	return ResourceAccess::Manager::GetInstance().Initialize();
}

bool ResourceAccess::Service::Process()
{
	return ResourceAccess::Manager::GetInstance().Process();
}

bool ResourceAccess::Service::Pause()
{
	return ResourceAccess::Manager::GetInstance().Pause();
}

void ResourceAccess::Service::Resume()
{
	ResourceAccess::Manager::GetInstance().Resume();
}

void ResourceAccess::Service::Terminate()
{
	ResourceAccess::Manager::GetInstance().Terminate();
}