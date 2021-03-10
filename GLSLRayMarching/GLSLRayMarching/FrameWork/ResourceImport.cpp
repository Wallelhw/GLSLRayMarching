//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "ResourceImport.h"

/////////////////////////////////////////////////////////////////////
ResourceImport::Manager::Manager()
{
}

ResourceImport::Manager::~Manager()
{
}

ResourceImport::Manager& ResourceImport::Manager::GetInstance()
{
	static ResourceImport::Manager instance;

	return instance;
}

bool ResourceImport::Manager::Initialize()
{
	return true;
}

bool ResourceImport::Manager::Process()
{
	return true;
}

bool ResourceImport::Manager::Pause()
{
	return true;
}

void ResourceImport::Manager::Resume()
{
}

void ResourceImport::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool ResourceImport::Service::Initialize()
{
	return ResourceImport::Manager::GetInstance().Initialize();
}

bool ResourceImport::Service::Process()
{
	return ResourceImport::Manager::GetInstance().Process();
}

bool ResourceImport::Service::Pause()
{
	return ResourceImport::Manager::GetInstance().Pause();
}

void ResourceImport::Service::Resume()
{
	ResourceImport::Manager::GetInstance().Resume();
}

void ResourceImport::Service::Terminate()
{
	ResourceImport::Manager::GetInstance().Terminate();
}