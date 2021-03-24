#include "FrameWork.h"
#include "Service.h"
#include "Video.h"

/////////////////////////////////////////////////////////////////////
class FrameWorkImpl
{
public:
	FrameWorkImpl()
	{
	}
};

FrameWork* FrameWork::instance = nullptr;

FrameWork::FrameWork()
: impl(nullptr)
, state(FrameWork::State::Instantiate)
{
	assert(!instance);
	instance = this;

	impl = new FrameWorkImpl();
}

FrameWork::~FrameWork()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}

	instance = nullptr;
}

bool FrameWork::Instantiate(int width_, int height_, const std::string& appName_, const std::string& initialScene_)
{
	if (!Platform::Instantiate(width_, height_, appName_.c_str(), initialScene_.c_str()))
		return false;

	if (!ServiceManager::GetInstance().Initialize())
	{
		return false;
	}

	if (!OnInstantiate())
	{
		Debug("FrameWork::Initiate Failed.\n");
		return false;
	}

	state = FrameWork::State::Update;

	return true;
}

bool FrameWork::Start()
{
	while (!Platform::ShouldAppQuit())
	{
		if (state == FrameWork::State::Update)
		{
			if (Platform::GetKeyDown(Platform::KeyCode::Pause))
			{
				ServiceManager::GetInstance().Pause();

				state = FrameWork::State::Pause;
			}

			if (!Platform::PreUpdate())
				return false;

			if (!ServiceManager::GetInstance().Update())
				return false;

			if (!OnUpdate())
				return false;

			if (!Platform::PostUpdate())
				return false;
		}
		else if (state == FrameWork::State::Pause)
		{
			if (Platform::GetKeyDown(Platform::KeyCode::Pause))
			{
				ServiceManager::GetInstance().Resume();

				state = FrameWork::State::Update;
			}

			if (!Platform::PreUpdate())
				return false;

			if (!Platform::PostUpdate())
				return false;
		}
	}

	state = FrameWork::State::Terminate;

	Platform::QuitApp();

	return true;
}

void FrameWork::Terminate()
{
	OnTerminate();

	ServiceManager::GetInstance().Terminate();

	Platform::Terminate();
}

FrameWork& FrameWork::GetInstance()
{
	return *instance;
}