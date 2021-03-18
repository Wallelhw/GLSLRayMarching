#include "TestCamera.h"

//////////////////////////////////////////////////////////////
TestCamera::TestCamera(GameObject& gameObject_)
: CameraComponent(gameObject_)
{
}

TestCamera::~TestCamera()
{
}

void TestCamera::OnRender()
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