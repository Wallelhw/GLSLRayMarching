#ifndef _IK2DBase_h_
#define _IK2DBase_h_

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class IK2DBase
{
public:
	IK2DBase()
	{
	}

	virtual ~IK2DBase()
	{
	}

	bool Create()
	{
		return OnCreate();
	}

	void Update(const Vector2& targetPosition)
	{
		OnUpdate(targetPosition);
	}

	void Destroy()
	{
		OnDestroy();
	}
protected:
	virtual bool OnCreate() = 0;
	virtual void OnUpdate(const Vector2& targetPosition) = 0;
	virtual void OnDestroy() = 0;
private:
};

#endif