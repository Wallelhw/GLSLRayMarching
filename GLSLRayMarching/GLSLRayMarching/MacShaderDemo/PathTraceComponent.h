#ifndef _PathTraceComponent_h_ 
#define _PathTraceComponent_h_ 

#include "Component.h"
#include "Video.h"
#include "PathTrace.h"

class PathTraceComponent : public Video::Graphics3Component
{
public:
	PathTraceComponent(GameObject& gameObject_);

	virtual ~PathTraceComponent();

	virtual void OnRender() override;

	virtual bool OnInitiate() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnTerminate() override;
private:
	PathTrace pathTrace;
};

#endif