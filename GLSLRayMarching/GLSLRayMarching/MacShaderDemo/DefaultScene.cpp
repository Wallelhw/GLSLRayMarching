//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "DefaultScene.h"

DefaultScene::DefaultScene()
{
}

DefaultScene::~DefaultScene()
{
}

bool DefaultScene::OnConstruct()
{
	return true;
}

bool DefaultScene::OnStart()
{
	return true;
}

bool DefaultScene::OnUpdate()
{
	
	/*
	static int a = 0;

	if (a == 0)
	{
		Push("Mac1");
		a = 1;
	}
	else if (a == 1)
	{
		Push("Mac2");
		a = 0;
	}
	*/

	return Push("Mac1");
}

bool DefaultScene::OnPause()
{
	return true;
}

void DefaultScene::OnResume()
{
}

void DefaultScene::OnStop()
{
}

void DefaultScene::OnDestruct()
{
}