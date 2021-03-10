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
{
}

Mac1Scene::~Mac1Scene()
{
}

bool Mac1Scene::OnConstruct()
{
	gameObject.Add(&testComponent);

	return true;
}

bool Mac1Scene::OnStart()
{
	return true;
}

bool Mac1Scene::OnUpdate()
{
	/*
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
	*/

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