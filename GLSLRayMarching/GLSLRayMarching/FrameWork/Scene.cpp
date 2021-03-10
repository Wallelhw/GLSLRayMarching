//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Scene.h"
#include "GameObject.h"

//////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::Construct()
{
	Platform::Debug("%sScene::Construct()\n", GetCurrentSceneName().c_str());
	if (!GameObject::Manager::GetInstance().Construct())
		return false;

	return OnConstruct();
}

bool Scene::Start()
{
	Platform::Debug("%sScene::Start()\n", GetCurrentSceneName().c_str());
	if (!GameObject::Manager::GetInstance().Start())
		return false;

	return OnStart();
}

bool Scene::Update()
{
	Platform::Debug("%sScene::Update()\n", GetCurrentSceneName().c_str());
	if (!GameObject::Manager::GetInstance().Update())
		return false;

	return OnUpdate();
}

bool Scene::Pause()
{
	Platform::Debug("%sScene::Pause()\n", GetCurrentSceneName().c_str());
	if (!GameObject::Manager::GetInstance().Pause())
		return false;

	return OnPause();
}

void Scene::Stop()
{
	Platform::Debug("%sScene::Stop()\n", GetCurrentSceneName().c_str());
	GameObject::Manager::GetInstance().Resume();
	
	OnStop();
}

void Scene::Resume()
{
	Platform::Debug("%sScene::Resume()\n", GetCurrentSceneName().c_str());
	GameObject::Manager::GetInstance().Stop();

	OnResume();
}

void Scene::Destruct()
{
	Platform::Debug("%sScene::Destruct()\n", GetCurrentSceneName().c_str());
	GameObject::Manager::GetInstance().Destruct();

	OnDestruct();
}

bool Scene::Push(const char* name_)
{
	return Scene::Manager::GetInstance().Push(name_);
}

bool Scene::Goto(const char* name_)
{
	return Scene::Manager::GetInstance().Goto(name_);
}

bool Scene::Pop()
{
	return Scene::Manager::GetInstance().Pop();
}

const std::string& Scene::GetCurrentSceneName()
{
	return Scene::Manager::GetInstance().GetCurrentSceneName();
}


//////////////////////////////////////////////////////////////////////////////////
Scene::ICreator::ICreator(const char* name_)
: name(name_)
{
	Scene::Manager::GetInstance().Add(this);
}

Scene::ICreator::~ICreator()
{
	Scene::Manager::GetInstance().Remove(this);
}

const std::string& Scene::ICreator::GetName() const
{
	return name;
}

//////////////////////////////////////////////////////////////////////////////////
Scene::Manager::Manager()
: sceneCreators()
, sceneStack()
, currentCreator(nullptr)
, nextCreator(nullptr)
, currentScene(nullptr)
{
}

Scene::Manager::~Manager()
{
}

Scene::Manager& Scene::Manager::GetInstance()
{
	static Scene::Manager instance;

	return instance;
}

bool Scene::Manager::Initialize()
{
	nextCreator = Find("Default");

	return true;
}

bool Scene::Manager::Process()
{
	if (nextCreator)
	{
		if (currentScene)
		{
			Platform::Debug("End Current Scene %s------------------------\n", currentScene->GetCurrentSceneName().c_str());
			currentScene->Destruct();

			delete currentScene;
			currentScene = nullptr;
		}

		Platform::Debug("Start Next Scene %s------------------------\n", nextCreator->GetName().c_str());
		currentScene = nextCreator->Create();
		currentCreator = nextCreator;
		nextCreator = nullptr;

		Assert(currentScene);

		if (!currentScene->Construct())
		{
			Platform::Error("failed to Construct %s\n", GetCurrentSceneName());
			return false;
		}

		if (!currentScene->Start())
		{
			Platform::Error("failed to Construct %s\n", GetCurrentSceneName());
			return false;
		}
	}
	else
	{
		// do nothing;
	}
	
	if (currentScene)
		currentScene->Update();

	return true;
}

bool Scene::Manager::Pause()
{
	if (currentScene)
		currentScene->Pause();

	return true;
}

void Scene::Manager::Resume()
{
	if (currentScene)
		currentScene->Resume();
}

void Scene::Manager::Terminate()
{
	if (currentScene)
		currentScene->Destruct();
}

void Scene::Manager::Add(ICreator* iCreator)
{
	auto itr = std::find(sceneCreators.begin(), sceneCreators.end(), iCreator);
	if (itr != sceneCreators.end())
	{
		Platform::Error("duplicated Scene Creator is declared\n");
		return;
	}

	sceneCreators.push_back(iCreator);
}

void Scene::Manager::Remove(ICreator* iCreator)
{
	auto itr = std::find(sceneCreators.begin(), sceneCreators.end(), iCreator);
	if (itr != sceneCreators.end())
		Scene::Manager::GetInstance().sceneCreators.erase(itr);
}

Scene::ICreator* Scene::Manager::Find(const char* name_) const
{
	if (!name_)
		return nullptr;

	for (auto sceneCreator : sceneCreators)
	{
		if (sceneCreator->GetName() == name_)
			return sceneCreator;
	}

	return nullptr;
}

bool Scene::Manager::Push(const char* name_)
{
	nextCreator = Find(name_);
	if (!nextCreator)
	{
		Platform::Info("Cannot Find Scene. Push Scene failed\n");
		return false;
	}

	sceneStack.push_back(currentCreator);

	return nextCreator;
}

bool Scene::Manager::Goto(const char* name_)
{
	if (!Scene::Manager::Pop())
		return false;

	return Scene::Manager::Push(name_);
}

bool Scene::Manager::Pop()
{
	if (sceneStack.size()==0)
	{
		Platform::Info("Scene Stack contain only 1 scene. At least one Scene is required\n");
		return false;
	}

	nextCreator = sceneStack.back();
	Assert(nextCreator);

	sceneStack.pop_back();
	return true;
}

const std::string& Scene::Manager::GetCurrentSceneName() const
{
	Assert(currentCreator);
	
	return currentCreator->GetName();
}

/////////////////////////////////////////////////////////////////////
bool Scene::Service::Initialize()
{
	return Scene::Manager::GetInstance().Initialize();
}

bool Scene::Service::Process()
{
	return Scene::Manager::GetInstance().Process();
}

bool Scene::Service::Pause()
{
	return Scene::Manager::GetInstance().Pause();
}

void Scene::Service::Resume()
{
	Scene::Manager::GetInstance().Resume();
}

void Scene::Service::Terminate()
{
	Scene::Manager::GetInstance().Terminate();
}