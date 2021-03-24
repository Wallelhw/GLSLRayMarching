#ifndef _ShaderToyCamera_h_ 
#define _ShaderToyCamera_h_ 

#include "Component.h"
#include "Video.h"

class ShaderToyCamera : public Video::CameraComponent
{
public:
	ShaderToyCamera(GameObject& gameObject_);

	virtual ~ShaderToyCamera();

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