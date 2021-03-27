#include "LightFieldComponent.h"

//////////////////////////////////////////////////////////////
LightFieldComponent::LightFieldComponent(GameObject& gameObject_)
: Graphics3Component(gameObject_)
, cfg(std::make_shared<Config>())
, lightFieldRenderer(cfg)
{
}

LightFieldComponent::~LightFieldComponent()
{
}

bool LightFieldComponent::OnInitiate()
{
	return true;
}

bool LightFieldComponent::OnStart()
{
	std::string path = "G:/Play/GLSLRayMarching1/GLSLRayMarching/GLSLRayMarching/assets/light-fields/shop/config.cfg";
	
	cfg->open(path);
	lightFieldRenderer.open();
	lightFieldRenderer.resize();

	return true;
}

bool LightFieldComponent::OnUpdate()
{
	return true;
}

bool LightFieldComponent::OnPause()
{
	return true;
}

void LightFieldComponent::OnResume()
{
}

void LightFieldComponent::OnStop()
{
}

void LightFieldComponent::OnTerminate()
{
}

void LightFieldComponent::OnRender()
{
	lightFieldRenderer.draw_contents();
}