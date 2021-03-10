//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "GameObject.h"
#include "ID.h"

GameObject::Manager::Manager()
{
}

GameObject::Manager::~Manager()
{
}

GameObject::Manager& GameObject::Manager::GetInstance()
{
	static GameObject::Manager instance;

	return instance;
}

bool GameObject::Manager::Construct()
{
	Platform::Debug("GameObject::Manager::Construct()\n");

	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->Construct())
			return false;
	}

	return true;
}

bool GameObject::Manager::Start()
{
	Platform::Debug("GameObject::Manager::Start()\n");

	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->Start())
			return false;
	}

	return true;
}

bool GameObject::Manager::Update()
{
	Platform::Debug("GameObject::Manager::Update()\n");

	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->Update())
			return false;
	}

	return true;
}

bool GameObject::Manager::Pause()
{
	Platform::Debug("GameObject::Manager::Pause()\n");

	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->Pause())
			return false;
	}

	return true;
}

void GameObject::Manager::Resume()
{
	Platform::Debug("GameObject::Manager::Resume()\n");

	for (auto& gameObject : gameObjects)
	{
		gameObject->Resume();
	}
}

void GameObject::Manager::Stop()
{
	for (auto& gameObject : gameObjects)
	{
		gameObject->Stop();
	}
}

void GameObject::Manager::Destruct()
{
	Platform::Debug("GameObject::Manager::Destruct()\n");

	for (auto& gameObject : gameObjects)
	{
		gameObject->Destruct();
	}
}

void GameObject::Manager::Add(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

void GameObject::Manager::Remove(GameObject* gameObject)
{
	auto itr = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (itr != gameObjects.end())
		gameObjects.erase(itr);
}

//////////////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
: Component::Owner(ID::Get())
{
	GameObject::Manager::GetInstance().Add(this);
}

GameObject::~GameObject()
{
	GameObject::Manager::GetInstance().Remove(this);
}

bool GameObject::OnConstruct()
{
	return true;
}

bool GameObject::OnStart()
{
	return true;
}

bool GameObject::OnUpdate()
{
	return true;
}

bool GameObject::OnPause()
{
	return true;
}

void GameObject::OnResume()
{
}

void GameObject::OnStop()
{
}

void GameObject::OnDestruct()
{
}