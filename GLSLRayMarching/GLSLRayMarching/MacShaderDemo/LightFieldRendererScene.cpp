//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "LightFieldRendererScene.h"
#include "Input.h"

LightFieldRendererScene::LightFieldRendererScene()
: lightFieldCamera(gameObject)
, lightFieldComponent(gameObject)
{
}

LightFieldRendererScene::~LightFieldRendererScene()
{
}

bool LightFieldRendererScene::OnInitiate()
{
	return true;
}

bool LightFieldRendererScene::OnStart()
{
	return true;
}

bool LightFieldRendererScene::OnUpdate()
{
	return true;
}

bool LightFieldRendererScene::OnPause()
{
	return true;
}

void LightFieldRendererScene::OnResume()
{
}

void LightFieldRendererScene::OnStop()
{
}

void LightFieldRendererScene::OnTerminate()
{
}