#ifndef _LightFieldCamera_h_ 
#define _LightFieldCamera_h_ 

#include "Component.h"
#include "Video.h"

class LightFieldCamera : public Video::CameraComponent
{
public:
	LightFieldCamera(GameObject& gameObject_);

	virtual ~LightFieldCamera();

	virtual void OnRender() override;

	virtual bool OnInitiate() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnTerminate() override;
private:
};

#endif