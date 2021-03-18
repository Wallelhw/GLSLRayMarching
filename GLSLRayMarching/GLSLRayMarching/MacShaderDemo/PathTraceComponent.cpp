#include "PathTraceComponent.h"

//////////////////////////////////////////////////////////////
PathTraceComponent::PathTraceComponent(GameObject& gameObject_)
: Graphics3Component(gameObject_)
{
}

PathTraceComponent::~PathTraceComponent()
{
}

void PathTraceComponent::OnRender()
{
	//pathTrace.Render();
}

bool PathTraceComponent::OnConstruct()
{
	return true;
}

bool PathTraceComponent::OnStart()
{
	//if (!pathTrace.Construct("../assets/cornell_box.scene"))
	//if (!pathTrace.Construct("../assets/bedroom.scene"))
	//if (!pathTrace.Construct("../assets/diningroom.scene"))
		//return false;
	
	return true;
}

bool PathTraceComponent::OnUpdate()
{
	return true;
}

bool PathTraceComponent::OnPause()
{
	return true;
}

void PathTraceComponent::OnResume()
{
}

void PathTraceComponent::OnStop()
{
}

void PathTraceComponent::OnDestruct()
{
}