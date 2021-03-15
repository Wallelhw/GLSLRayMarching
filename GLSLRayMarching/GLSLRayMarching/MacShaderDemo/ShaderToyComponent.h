#ifndef _ShaderToyComponent_h_ 
#define _ShaderToyComponent_h_ 

#include "Component.h"
#include "Video.h"

class MacShaderDemo;

class ShaderToyComponent : public Video::Graphics3Component
{
public:
	ShaderToyComponent(GameObject& gameObject_);

	virtual ~ShaderToyComponent();

	virtual void OnRender() override;

	virtual bool OnConstruct() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnDestruct() override;
private:
	MacShaderDemo* macShaderDemo;
};

#endif