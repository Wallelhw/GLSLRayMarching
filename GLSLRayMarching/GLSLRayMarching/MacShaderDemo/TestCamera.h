#ifndef _TestCamera_h_ 
#define _TestCamera_h_ 

#include "Component.h"
#include "Video.h"

class TestCamera : public Video::CameraComponent
{
public:
	TestCamera(GameObject& gameObject_);

	virtual ~TestCamera();

	virtual void OnRender() override;

	virtual bool OnConstruct() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnDestruct() override;
private:
};

#endif