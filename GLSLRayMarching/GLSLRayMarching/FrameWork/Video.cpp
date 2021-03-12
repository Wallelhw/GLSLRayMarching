//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Video.h" 

#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Primitives.h"
#include "RenderStates.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GUI.h"

/////////////////////////////////////////////////////////////////////
Video::Graphics3Component::Graphics3Component(Component::Owner& owner_)
	: Component(owner_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::Graphics3Component::~Graphics3Component()
{
	Video::Manager::GetInstance().Remove(this);
}

bool Video::Graphics3Component::OnConstruct()
{
	return true;
}

bool Video::Graphics3Component::OnStart()
{
	return true;
}

bool Video::Graphics3Component::OnUpdate()
{
	return true;
}

bool Video::Graphics3Component::OnPause()
{
	return true;
}

void Video::Graphics3Component::OnResume()
{
}

void Video::Graphics3Component::OnStop()
{
}

void Video::Graphics3Component::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
Video::RendererComponent::RendererComponent(Component::Owner& owner_)
	: Component(owner_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::RendererComponent::~RendererComponent()
{
	Video::Manager::GetInstance().Remove(this);
}

bool Video::RendererComponent::OnConstruct()
{
	return true;
}

bool Video::RendererComponent::OnStart()
{
	return true;
}

bool Video::RendererComponent::OnUpdate()
{
	return true;
}

bool Video::RendererComponent::OnPause()
{
	return true;
}

void Video::RendererComponent::OnResume()
{
}

void Video::RendererComponent::OnStop()
{
}

void Video::RendererComponent::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
Video::CameraComponent::CameraComponent(Component::Owner& owner_)
	: Component(owner_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::CameraComponent::~CameraComponent()
{
	Video::Manager::GetInstance().Remove(this);
}

bool Video::CameraComponent::OnConstruct()
{
	return true;
}

bool Video::CameraComponent::OnStart()
{
	return true;
}

bool Video::CameraComponent::OnUpdate()
{
	return true;
}

bool Video::CameraComponent::OnPause()
{
	return true;
}

void Video::CameraComponent::OnResume()
{
}

void Video::CameraComponent::OnStop()
{
}

void Video::CameraComponent::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
Video::Manager::Manager()
{
}

Video::Manager::~Manager()
{
}

Video::Manager& Video::Manager::GetInstance()
{
	static Video::Manager instance;

	return instance;
}

bool Video::Manager::Initialize()
{
	return true;
}

bool Video::Manager::Process()
{
	for (auto& camera : cameras)
	{
		for (auto& renderer : renderers)
		{
		}
	}

	return true;
}

bool Video::Manager::Pause()
{
	return true;
}

void Video::Manager::Resume()
{
}

void Video::Manager::Terminate()
{
	cameras.clear();
	renderers.clear();
	graphics3s.clear();
}

void Video::Manager::Add(CameraComponent* camera)
{
	auto itr = std::find(cameras.begin(), cameras.end(), camera);
	if (itr != cameras.end())
	{
		Platform::Error("duplicated CameraComponent is declared\n");
		return;
	}

	cameras.push_back(camera);
}

void Video::Manager::Add(RendererComponent* renderer)
{
	auto itr = std::find(renderers.begin(), renderers.end(), renderer);
	if (itr != renderers.end())
	{
		Platform::Error("duplicated RendererComponent is declared\n");
		return;
	}

	renderers.push_back(renderer);
}

void Video::Manager::Add(Graphics3Component* graphics3)
{
	auto itr = std::find(graphics3s.begin(), graphics3s.end(), graphics3);
	if (itr != graphics3s.end())
	{
		Platform::Error("duplicated Graphics3Component is declared\n");
		return;
	}

	graphics3s.push_back(graphics3);
}

void Video::Manager::Remove(CameraComponent* camera)
{
	auto itr = std::find(cameras.begin(), cameras.end(), camera);
	if (itr != cameras.end())
		cameras.erase(itr);
}

void Video::Manager::Remove(RendererComponent* renderer)
{
	auto itr = std::find(renderers.begin(), renderers.end(), renderer);
	if (itr != renderers.end())
		renderers.erase(itr);
}

void Video::Manager::Remove(Graphics3Component* graphics3)
{
	auto itr = std::find(graphics3s.begin(), graphics3s.end(), graphics3);
	if (itr != graphics3s.end())
		graphics3s.erase(itr);
}

/////////////////////////////////////////////////////////////////////
bool Video::Service::Initialize()
{
	return Video::Manager::GetInstance().Initialize();
}

bool Video::Service::Process()
{
	return Video::Manager::GetInstance().Process();
}

bool Video::Service::Pause()
{
	return Video::Manager::GetInstance().Pause();
}

void Video::Service::Resume()
{
	Video::Manager::GetInstance().Resume();
}

void Video::Service::Terminate()
{
	Video::Manager::GetInstance().Terminate();
}