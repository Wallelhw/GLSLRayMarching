//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Mac2Scene.h"

Mac2Scene::Mac2Scene()
{
}

Mac2Scene::~Mac2Scene()
{
}

bool Mac2Scene::OnConstruct()
{
	return true;
}

bool Mac2Scene::OnStart()
{
	return true;
}

bool Mac2Scene::OnUpdate()
{
	static int aa = 0;

	if (aa == 0)
	{
		Push("Mac3");
		aa = 1;
	}
	else if (aa == 1)
	{
		Pop();

		aa = 0;
	}

	return true;
}

bool Mac2Scene::OnPause()
{
	return true;
}

void Mac2Scene::OnResume()
{
}

void Mac2Scene::OnStop()
{
}

void Mac2Scene::OnDestruct()
{
}