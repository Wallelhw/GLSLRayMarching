//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Video.h"

#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Primitives.h"
#include "RenderStates.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GUI.h"

/////////////////////////////////////////////////////////////////////
Video::Manager::Manager()
{
}

Video::Manager::~Manager()
{
}

Video::Manager& Video::Manager::GetInstance()
{
	static Video::Manager instance;

	return instance;
}

bool Video::Manager::Initialize()
{
	return true;
}

bool Video::Manager::Process()
{
	return true;
}

bool Video::Manager::Pause()
{
	return true;
}

void Video::Manager::Resume()
{
}

void Video::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Video::Service::Initialize()
{
	return Video::Manager::GetInstance().Initialize();
}

bool Video::Service::Process()
{
	return Video::Manager::GetInstance().Process();
}

bool Video::Service::Pause()
{
	return Video::Manager::GetInstance().Pause();
}

void Video::Service::Resume()
{
	Video::Manager::GetInstance().Resume();
}

void Video::Service::Terminate()
{
	Video::Manager::GetInstance().Terminate();
}