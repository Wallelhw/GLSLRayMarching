#ifndef _LightFieldComponent_h_ 
#define _LightFieldComponent_h_ 

#include "Component.h"
#include "Video.h"
#include "light-field-renderer.h"
#include "Config.h"

class LightFieldComponent : public Video::Graphics3Component
{
public:
	LightFieldComponent(GameObject& gameObject_);

	virtual ~LightFieldComponent();

	virtual void OnRender() override;

	virtual bool OnInitiate() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnTerminate() override;
private:
	std::shared_ptr<Config> cfg;
	LightFieldRenderer lightFieldRenderer;
};

#endif