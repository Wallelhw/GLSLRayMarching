//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "ID.h"
#include "Component.h"
#include "GameObject.h"

///////////////////////////////////////////////////////////////
Component::Component(GameObject& gameObject_, const char* name_)
: gameObject(&gameObject_)
, id(ID::Get())
, name(name_)
{
	GetGameObject().Add(this);
}

Component::~Component()
{
	GetGameObject().Remove(this);
}

GameObject& Component::GetGameObject()
{
	return *gameObject;
}

const GameObject& Component::GetGameObject() const
{
	return *gameObject;
}

ID Component::GetID() const
{
	return id;
}

const std::string& Component::GetName() const
{
	return name;
}

void Component::SetName(const std::string& name_)
{
	name = name_;
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