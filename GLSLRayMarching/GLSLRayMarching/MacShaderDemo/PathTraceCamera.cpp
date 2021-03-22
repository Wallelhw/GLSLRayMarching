#include "PathTraceCamera.h"
#include "Video.h"
#include "RenderStates.h"

//////////////////////////////////////////////////////////////
PathTraceCamera::PathTraceCamera(GameObject& gameObject_)
: CameraComponent(gameObject_)
{
}

PathTraceCamera::~PathTraceCamera()
{
}

bool PathTraceCamera::OnConstruct()
{
	return true;
}

bool PathTraceCamera::OnStart()
{
	return true;
}

bool PathTraceCamera::OnUpdate()
{
	return true;
}

bool PathTraceCamera::OnPause()
{
	return true;
}

void PathTraceCamera::OnResume()
{
}

void PathTraceCamera::OnStop()
{
}

void PathTraceCamera::OnDestruct()
{
}

void PathTraceCamera::OnRender()
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
