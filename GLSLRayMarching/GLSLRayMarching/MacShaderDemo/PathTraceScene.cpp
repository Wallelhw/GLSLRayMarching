//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "PathTraceScene.h"
#include "Input.h"

PathTraceScene::PathTraceScene()
	: pathTraceCamera(gameObject)
	, pathTraceComponent(gameObject)
{
}

PathTraceScene::~PathTraceScene()
{
}

bool PathTraceScene::OnInitiate()
{
	return true;
}

bool PathTraceScene::OnStart()
{
	return true;
}

bool PathTraceScene::OnUpdate()
{
	if (Input::Manager::GetInstance().GetKeyHold(Platform::KeyCode::KeypadEnter))
		Debug("Key is Held\n");
	if (Input::Manager::GetInstance().GetKeyDown(Platform::KeyCode::KeypadEnter))
		Debug("Key is Down\n");
	if (Input::Manager::GetInstance().GetKeyUp(Platform::KeyCode::KeypadEnter))
		Debug("Key is Up\n");

	if (Input::Manager::GetInstance().GetMouseButtonHold(1))
		Debug("Mouse is Held\n");
	if (Input::Manager::GetInstance().GetMouseButtonDown(1))
		Debug("Mouse is Down\n");
	if (Input::Manager::GetInstance().GetMouseButtonUp(1))
		Debug("Mouse is Up\n");

	return true;
}

bool PathTraceScene::OnPause()
{
	return true;
}

void PathTraceScene::OnResume()
{
}

void PathTraceScene::OnStop()
{
}

void PathTraceScene::OnTerminate()
{
}