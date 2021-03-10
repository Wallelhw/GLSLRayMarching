//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Input.h"

/////////////////////////////////////////////////////////////////////
Input::Manager::Manager()
{
}

Input::Manager::~Manager()
{
}

Input::Manager& Input::Manager::GetInstance()
{
	static Input::Manager instance;

	return instance;
}

bool Input::Manager::Initialize()
{
	return true;
}

bool Input::Manager::Process()
{
	return true;
}

bool Input::Manager::Pause()
{
	return true;
}

void Input::Manager::Resume()
{
}

void Input::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Input::Service::Initialize()
{
	return Input::Manager::GetInstance().Initialize();
}

bool Input::Service::Process()
{
	return Input::Manager::GetInstance().Process();
}

bool Input::Service::Pause()
{
	return Input::Manager::GetInstance().Pause();
}

void Input::Service::Resume()
{
	Input::Manager::GetInstance().Resume();
}

void Input::Service::Terminate()
{
	Input::Manager::GetInstance().Terminate();
}