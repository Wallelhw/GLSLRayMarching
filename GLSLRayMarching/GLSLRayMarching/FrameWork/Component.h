//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Component_h_
#define _Component_h_

#include "Platform.h"
#include "Frame3.h"
#include "ID.h"

class GameObject;

class Component
{
	friend class GameObject;
///////////////////////////////////////////////////////////////
public:
	Component(GameObject& gameObject_, const char* name_ = "");
	
	virtual ~Component();

	GameObject& GetGameObject();

	const GameObject& GetGameObject() const;

	ID GetID() const;

	const std::string& GetName() const;

	void SetName(const std::string& name_);
private:
	bool Construct();

	bool Start();

	bool Update();

	bool Pause();

	void Resume();

	void Stop();

	void Destruct();
protected:
	virtual bool OnConstruct() = 0;

	virtual bool OnStart() = 0;

	virtual bool OnUpdate() = 0;

	virtual bool OnPause() = 0;

	virtual void OnResume() = 0;

	virtual void OnStop() = 0;

	virtual void OnDestruct() = 0;
private:
	ID id;
	std::string name;
	GameObject* gameObject;
///////////////////////////////////////////////////////////////
public:
};

#endif