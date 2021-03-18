//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Service.h"

IService::IService(const char* name_)
	: name(name_)
{
	ServiceManager::GetInstance().Add(this);
}

IService::~IService()
{
	ServiceManager::GetInstance().Remove(this);
}

const char* IService::GetName()
{
	return name;
}

//////////////////////////////////////////////////////////////////////////////////
ServiceManager::ServiceManager()
{
}

ServiceManager::~ServiceManager()
{
}

ServiceManager& ServiceManager::GetInstance()
{
	static ServiceManager instance;

	return instance;
}

bool ServiceManager::Initialize()
{
	for (auto& service : services)
	{
		if (!service->Initialize())
		{
			return false;
		}

		Debug("Service %s: Initialized\n", service->GetName());
	}

	return true;
}

bool ServiceManager::Process()
{
	for (auto& service : services)
	{
		if (!service->Process())
			return false;
	}

	return true;
}

bool ServiceManager::Pause()
{
	for (auto& service : services)
	{
		if (!service->Pause())
			return false;

		Debug("Service %s: Paused\n", service->GetName());
	}

	return true;
}

void ServiceManager::Resume()
{
	for (auto& service : services)
	{
		service->Resume();

		Debug("Service %s: Resumed\n", service->GetName());
	}
}

void ServiceManager::Terminate()
{
	for (auto& service : services)
	{
		service->Terminate();

		Debug("Service %s: Terminated\n", service->GetName());
	}
}

void ServiceManager::Add(IService* service)
{
	services.push_back(service);
}

void ServiceManager::Remove(IService* service)
{
	auto itr = std::find(services.begin(), services.end(), service);
	if (itr != services.end())
		services.erase(itr);
}