//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Mac3Scene.h"

Mac3Scene::Mac3Scene()
{
}

Mac3Scene::~Mac3Scene()
{
}

bool Mac3Scene::OnConstruct()
{
	return true;
}

bool Mac3Scene::OnStart()
{
	return true;
}

bool Mac3Scene::OnUpdate()
{
	Pop();

	return true;
}

bool Mac3Scene::OnPause()
{
	return true;
}

void Mac3Scene::OnResume()
{
}

void Mac3Scene::OnStop()
{
}

void Mac3Scene::OnDestruct()
{
}