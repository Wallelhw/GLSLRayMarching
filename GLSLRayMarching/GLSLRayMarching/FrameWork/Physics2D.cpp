//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Physics2D.h"

/////////////////////////////////////////////////////////////////////
Physics2D::Manager::Manager()
{
}

Physics2D::Manager::~Manager()
{
}

Physics2D::Manager& Physics2D::Manager::GetInstance()
{
	static Physics2D::Manager instance;

	return instance;
}

bool Physics2D::Manager::Initialize()
{
	return true;
}

bool Physics2D::Manager::Update()
{
	return true;
}

bool Physics2D::Manager::Pause()
{
	return true;
}

void Physics2D::Manager::Resume()
{
}

void Physics2D::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Physics2D::Service::Initialize()
{
	return Physics2D::Manager::GetInstance().Initialize();
}

bool Physics2D::Service::Update()
{
	return Physics2D::Manager::GetInstance().Update();
}

bool Physics2D::Service::Pause()
{
	return Physics2D::Manager::GetInstance().Pause();
}

void Physics2D::Service::Resume()
{
	Physics2D::Manager::GetInstance().Resume();
}

void Physics2D::Service::Terminate()
{
	Physics2D::Manager::GetInstance().Terminate();
}