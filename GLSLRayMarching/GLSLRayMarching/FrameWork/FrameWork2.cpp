#include "FrameWork2.h"
#include "Service.h"
#include "Video.h"

/////////////////////////////////////////////////////////////////////
class FrameWork2Impl
{
public:
	FrameWork2Impl(const std::string& name_)
		: name(name_)
	{
	}

	std::string name;
};

FrameWork2* FrameWork2::instance = nullptr;

FrameWork2::FrameWork2(const std::string& name_)
: impl(nullptr)
{
	assert(!instance);
	instance = this;

	impl = new FrameWork2Impl(name_);
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

bool FrameWork2::Create()
{
	if (!Platform::Instantiate())
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
	while (!Video::Manager::GetInstance().ShouldClose())
	{
		//Platform::Pause();
		//Platform::Resume();
		if (!Platform::Update())
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