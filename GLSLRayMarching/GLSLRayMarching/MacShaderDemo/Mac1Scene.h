//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Mac1Scene_h_
#define _Mac1Scene_h_

#include "Platform.h"
#include "Scene.h"
#include "GameObject.h"
#include "TestCamera.h"
#include "ShaderToyComponent.h"
#include "PathTraceComponent.h"

class Mac1Scene : public Scene
{
public:
	Mac1Scene();

	virtual ~Mac1Scene();
protected:
	virtual bool OnConstruct() override;

	virtual bool OnStart() override;

	virtual bool OnUpdate() override;

	virtual bool OnPause() override;

	virtual void OnResume() override;

	virtual void OnStop() override;

	virtual void OnDestruct() override;

private:
	GameObject gameObject;
	TestCamera testCamera;
	//ShaderToyComponent shaderToyComponent;
	PathTraceComponent pathTraceComponent;
};

#endif