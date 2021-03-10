//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Physics3D.h"

/////////////////////////////////////////////////////////////////////
Physics3D::Manager::Manager()
{
}

Physics3D::Manager::~Manager()
{
}

Physics3D::Manager& Physics3D::Manager::GetInstance()
{
	static Physics3D::Manager instance;

	return instance;
}

bool Physics3D::Manager::Initialize()
{
	return true;
}

bool Physics3D::Manager::Process()
{
	return true;
}

bool Physics3D::Manager::Pause()
{
	return true;
}

void Physics3D::Manager::Resume()
{
}

void Physics3D::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Physics3D::Service::Initialize()
{
	return Physics3D::Manager::GetInstance().Initialize();
}

bool Physics3D::Service::Process()
{
	return Physics3D::Manager::GetInstance().Process();
}

bool Physics3D::Service::Pause()
{
	return Physics3D::Manager::GetInstance().Pause();
}

void Physics3D::Service::Resume()
{
	Physics3D::Manager::GetInstance().Resume();
}

void Physics3D::Service::Terminate()
{
	Physics3D::Manager::GetInstance().Terminate();
}