#ifndef _FrameWork_h_
#define _FrameWork_h_

#include "Platform.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class FrameWorkImpl;

class FrameWork
{
public:
	enum class State
	{
		Instantiate = 0,
		Update,
		Pause,
		Terminate
	};
	FrameWork();
	virtual ~FrameWork();

	bool Instantiate(int width_, int height_, const std::string& appName_, const std::string& initialScene_);
	bool Start();
	void Terminate();

	static FrameWork& GetInstance();
protected:
	virtual bool OnInstantiate() = 0;
	virtual bool OnUpdate() = 0;
	virtual void OnTerminate() = 0;
private:
	static FrameWork* instance;
	FrameWorkImpl* impl;

	State state;
public:
protected:
private:
};

#endif