#ifndef _PathTraceCamera_h_ 
#define _PathTraceCamera_h_ 

#include "Component.h"
#include "Video.h"

class PathTraceCamera : public Video::CameraComponent
{
public:
	PathTraceCamera(GameObject& gameObject_);

	virtual ~PathTraceCamera();

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