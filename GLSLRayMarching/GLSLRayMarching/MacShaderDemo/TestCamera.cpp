#include "TestCamera.h"
#include "Video.h"
#include "RenderStates.h"

//////////////////////////////////////////////////////////////
TestCamera::TestCamera(GameObject& gameObject_)
: CameraComponent(gameObject_)
{
}

TestCamera::~TestCamera()
{
}

bool TestCamera::OnConstruct()
{
	return true;
}

bool TestCamera::OnStart()
{
	return true;
}

bool TestCamera::OnUpdate()
{
	return true;
}

bool TestCamera::OnPause()
{
	return true;
}

void TestCamera::OnResume()
{
}

void TestCamera::OnStop()
{
}

void TestCamera::OnDestruct()
{
}

void TestCamera::OnRender()
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
