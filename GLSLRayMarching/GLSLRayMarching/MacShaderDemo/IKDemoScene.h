#ifndef _IKDemoScene_h_
#define _IKDemoScene_h_

#include "Scene.h"
#include "GameObject.h"
#include "IKShapes.h"

class IKDemoScene : public Scene
{
public:
	IKDemoScene()
		: Scene()
		, ikCameraComponent(gameObject)
		//, ik2D2JointShape(gameObject, ikCameraComponent)
		//, ik2DNJointShape(gameObject, ikCameraComponent)
		//, ik2DCCDJointShape(gameObject, ikCameraComponent)
		, ik2DFABRIKJointShape(gameObject, ikCameraComponent)
	{
	}

	virtual ~IKDemoScene()
	{
	}
protected:
	virtual bool OnInitiate() override
	{
		return true;
	}

	virtual bool OnStart() override
	{
		return true;
	}

	virtual bool OnUpdate() override
	{
		return true;
	}

	virtual bool OnPause() override
	{
		return true;
	}

	virtual void OnResume() override
	{
	}

	virtual void OnStop() override
	{
	}

	virtual void OnTerminate() override
	{
	}
private:
	GameObject gameObject;
	IKCameraComponent ikCameraComponent;
	// IK2D2JointShape ik2D2JointShape;
	// IK2DNJointShape ik2DNJointShape;
	// IK2DCCDJointShape ik2DCCDJointShape;
	IK2DFABRIKJointShape ik2DFABRIKJointShape;
};

#endif