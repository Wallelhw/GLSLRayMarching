//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Component.h"

Component::Owner::Owner(ID id_)
: id(id_)
, name()
, components()
{
}

Component::Owner::~Owner()
{
}

void Component::Owner::Add(Component* component_)
{
	components.push_back(component_);
}

void Component::Owner::Remove(Component* component_)
{
	auto itr = std::find(components.begin(), components.end(), component_);
	
	components.erase(itr);
}

Component* Component::Owner::Get(ID id_)
{
	for (auto& component : components)
	{
		if (component->id == id_)
		{
			return component;
		}
	}

	return nullptr;
}

Component* Component::Owner::Get(const std::string& name_)
{
	for (auto& component : components)
	{
		if (component->name == name_)
		{
			return component;
		}
	}

	return nullptr;
}

int Component::Owner::GetComponentCount() const
{
	return components.size();
}

const std::string& Component::Owner::GetName() const
{
	return name;
}

ID Component::Owner::GetID() const
{
	return id;
}

bool Component::Owner::Construct()
{
	if (!OnConstruct())
		return false;

	for (auto& component : components)
	{
		if (!component->Construct())
		{
			return false;
		}
	}

	return true;
}

bool Component::Owner::Start()
{
	if (!OnStart())
		return false;

	for (auto& component : components)
	{
		if (!component->Start())
		{
			return false;
		}
	}

	return true;
}

bool Component::Owner::Update()
{
	if (!OnUpdate())
		return false;

	for (auto& component : components)
	{
		if (!component->Update())
		{
			return false;
		}
	}

	return true;
}

bool Component::Owner::Pause()
{
	if (!OnPause())
		return false;

	for (auto& component : components)
	{
		if (!component->Pause())
		{
			return false;
		}
	}

	return true;
}

void Component::Owner::Resume()
{
	OnResume();

	for (auto& component : components)
	{
		component->Resume();
	}
}

void Component::Owner::Stop()
{
	OnStop();

	for (auto& component : components)
	{
		component->Stop();
	}
}

void Component::Owner::Destruct()
{
	OnDestruct();

	for (auto& component : components)
	{
		component->Destruct();
	}
}

///////////////////////////////////////////////////////////////
Component::Component()
{
}

Component::~Component()
{
}

ID Component::GetID() const
{
	return id;
}

const std::string& Component::GetName() const
{
	return name;
}

bool Component::Construct()
{
	return OnConstruct();
}

bool Component::Start()
{
	return OnStart();
}

bool Component::Update()
{
	return OnUpdate();
}

bool Component::Pause()
{
	return OnPause();
}

void Component::Resume()
{
	OnResume();
}

void Component::Stop()
{
	OnStop();
}

void Component::Destruct()
{
	OnDestruct();
}