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

#include <rttr/registration>

class Component
{
public:
	class Owner : public Frame3
	{
	public:
		Owner(ID id);

		virtual ~Owner();

		void Add(Component* component_);

		void Remove(Component* component_);

		Component* Get(ID id);

		Component* Get(const std::string& name_);

		const std::string& GetName() const;

		ID GetID() const;

		int GetComponentCount() const;
	protected:
		bool Construct();

		bool Start();

		bool Update();

		bool Pause();

		void Stop();

		void Resume();

		void Destruct();
	protected:
		virtual bool OnConstruct() = 0;

		virtual bool OnStart() = 0;

		virtual bool OnUpdate() = 0;

		virtual bool OnPause() = 0;

		virtual void OnResume() = 0;

		virtual void OnStop() = 0;

		virtual void OnDestruct() = 0;

		RTTR_ENABLE()
	private:
		ID id;
		std::string name;
		std::vector<Component*> components;
	};

///////////////////////////////////////////////////////////////
public:
	Component();
	
	virtual ~Component();

	const std::string& GetName() const;

	ID GetID() const;
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
///////////////////////////////////////////////////////////////
public:
};

#endif