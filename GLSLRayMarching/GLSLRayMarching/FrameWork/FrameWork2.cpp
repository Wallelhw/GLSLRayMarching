#include "FrameWork2.h"
#include "Service.h"
#include "Video.h"

/////////////////////////////////////////////////////////////////////
class FrameWork2Impl
{
public:
	FrameWork2Impl()
	{
	}
};

FrameWork2* FrameWork2::instance = nullptr;

FrameWork2::FrameWork2()
: impl(nullptr)
{
	assert(!instance);
	instance = this;

	impl = new FrameWork2Impl();
}

FrameWork2::~FrameWork2()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}

	instance = nullptr;
}

bool FrameWork2::Create(int width_, int height_, const std::string& appName_, const std::string& initialScene_)
{
	if (!Platform::Instantiate(width_, height_, appName_.c_str(), initialScene_.c_str()))
		return false;

	if (!ServiceManager::GetInstance().Initialize())
	{
		return false;
	}

	if (!OnCreate())
	{
		Debug("FrameWork2::Create Failed.\n");
		return false;
	}

	return true;
}

bool FrameWork2::Start()
{
	while (!Platform::QuitApp())
	{
		//Platform::Pause();
		//Platform::Resume();
		if (!Platform::PreUpdate())
			return false;

		if (!ServiceManager::GetInstance().Process())
		{
			return false;
		}

		if (!OnUpdate())
			return false;

		//if (glData.updateState)
		//{
			//ServiceManager::GetInstance().Pause();
			//ServiceManager::GetInstance().Resume();
		//}

		if (!Platform::PostUpdate())
			return false;
	}

	return true;
}

void FrameWork2::Destroy()
{
	OnDestroy();

	ServiceManager::GetInstance().Terminate();

	Platform::Terminate();
}

FrameWork2& FrameWork2::GetInstance()
{
	return *instance;
}