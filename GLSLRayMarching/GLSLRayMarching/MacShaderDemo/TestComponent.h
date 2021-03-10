#ifndef _TestComponent_h_ 
#define _TestComponent_h_ 

#include "Component.h"

class MacShaderDemo;

class TestComponent : public Component
{
public:
	TestComponent();

	virtual ~TestComponent();

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