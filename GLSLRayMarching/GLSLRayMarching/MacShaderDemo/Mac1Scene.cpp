//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Mac1Scene.h"

Mac1Scene::Mac1Scene()
: testCamera(gameObject)
//, shaderToyComponent(gameObject)
, pathTraceComponent(gameObject)
{
}

Mac1Scene::~Mac1Scene()
{
}

bool Mac1Scene::OnConstruct()
{
	gameObject.Add(&testCamera);
	//gameObject.Add(&shaderToyComponent);
	gameObject.Add(&pathTraceComponent);

	return true;
}

bool Mac1Scene::OnStart()
{
	return true;
}

bool Mac1Scene::OnUpdate()
{
	return true;
}

bool Mac1Scene::OnPause()
{
	return true;
}

void Mac1Scene::OnResume()
{
}

void Mac1Scene::OnStop()
{
}

void Mac1Scene::OnDestruct()
{
}