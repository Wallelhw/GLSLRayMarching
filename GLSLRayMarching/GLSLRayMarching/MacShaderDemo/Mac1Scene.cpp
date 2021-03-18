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
, shaderToyComponent(gameObject)
//, pathTraceComponent(gameObject)
{
}

Mac1Scene::~Mac1Scene()
{
}

bool Mac1Scene::OnConstruct()
{
	gameObject.Add(&testCamera);
	gameObject.Add(&shaderToyComponent);
	//gameObject.Add(&pathTraceComponent);

	return true;
}

bool Mac1Scene::OnStart()
{
	return true;
}

#include "Input.h"

bool Mac1Scene::OnUpdate()
{
	if (Input::Manager::GetInstance().GetKey(Input::KeyCode::L))
		Debug("Key is Held");
	if (Input::Manager::GetInstance().GetKeyDown(Input::KeyCode::L))
		Debug("Key is Down");
	if (Input::Manager::GetInstance().GetKeyUp(Input::KeyCode::L))
		Debug("Key is Up");

	if (Input::Manager::GetInstance().GetMouseButtonDown(0))
		Debug("Mouse is Down");
	if (Input::Manager::GetInstance().GetMouseButtonUp(0))
		Debug("Mouse is Up");
	if (Input::Manager::GetInstance().GetMouseButton(0))
	{
		Vector2 v = Input::Manager::GetInstance().GetMouseMovement();
		Debug("Mouse is Held %f %f\n", v.X(), v.Y());
	}

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