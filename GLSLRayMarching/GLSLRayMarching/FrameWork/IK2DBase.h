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

	bool Initiate()
	{
		return OnInitiate();
	}

	void Update(const Vector2& targetPosition)
	{
		OnUpdate(targetPosition);
	}

	void Terminate()
	{
		OnTerminate();
	}
protected:
	virtual bool OnInitiate() = 0;
	virtual void OnUpdate(const Vector2& targetPosition) = 0;
	virtual void OnTerminate() = 0;
private:
};

#endif