#include "LightFieldCamera.h"
#include "Video.h"
#include "RenderStates.h"

//////////////////////////////////////////////////////////////
LightFieldCamera::LightFieldCamera(GameObject& gameObject_)
: CameraComponent(gameObject_)
{
}

LightFieldCamera::~LightFieldCamera()
{
}

bool LightFieldCamera::OnInitiate()
{
	return true;
}

bool LightFieldCamera::OnStart()
{
	return true;
}

bool LightFieldCamera::OnUpdate()
{
	return true;
}

bool LightFieldCamera::OnPause()
{
	return true;
}

void LightFieldCamera::OnResume()
{
}

void LightFieldCamera::OnStop()
{
}

void LightFieldCamera::OnTerminate()
{
}

void LightFieldCamera::OnRender()
{
	ClearState clearState;
	clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
	clearState.clearDepth = 1.0f;
	clearState.clearStencil = 0;
	clearState.enableClearColor = true;
	clearState.enableClearDepth = true;
	clearState.enableClearStencil = true;
	clearState.Apply();
}
