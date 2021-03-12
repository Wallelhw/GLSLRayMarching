#ifndef _FrameWork2_h_
#define _FrameWork2_h_

#include "Platform.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class FrameWork2Impl;

class FrameWork2
{
public:
	FrameWork2(const std::string& name_);
	virtual ~FrameWork2();

	bool Create();
	bool Start();
	void Destroy();

	static FrameWork2& GetInstance();
protected:
	virtual bool OnCreate() = 0;
	virtual bool OnUpdate() = 0;
	virtual void OnDestroy() = 0;
private:
	static FrameWork2* instance;
	FrameWork2Impl* impl;
public:
protected:
private:
};

#endif