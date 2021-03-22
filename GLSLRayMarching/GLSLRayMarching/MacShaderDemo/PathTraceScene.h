//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _PathTraceScene_h_
#define _PathTraceScene_h_

#include "Platform.h"
#include "Scene.h"
#include "GameObject.h"
#include "PathTraceCamera.h"
#include "ShaderToyComponent.h"
#include "PathTraceComponent.h"

class PathTraceScene : public Scene
{
public:
	PathTraceScene();

	virtual ~PathTraceScene();
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
	PathTraceCamera pathTraceCamera;
	PathTraceComponent pathTraceComponent;
};

#endif