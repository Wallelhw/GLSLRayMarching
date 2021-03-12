//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Service_h_
#define _Service_h_

#include "Platform.h"

#include "String.h"

class ServiceManager;

class IService
{
public:
	IService(const char* name_);

	virtual ~IService();

	const char* GetName();

	virtual bool Initialize() = 0;

	virtual bool Process() = 0;

	virtual bool Pause() = 0;

	virtual void Resume() = 0;

	virtual void Terminate() = 0;
protected:
	const char* name;
};

template<class T>
class Service : public IService
{
public:
	Service(const char* name_)
		: IService(name_)
	{
	}

	virtual ~Service()
	{
	}

	virtual bool Initialize()
	{
		return T::Service::Initialize();
	}

	bool Process()
	{
		return T::Service::Process();
	}

	bool Pause()
	{
		return T::Service::Pause();
	}

	void Resume()
	{
		T::Service::Resume();
	}

	void Terminate()
	{
		T::Service::Terminate();
	}
private:
};

class ServiceManager
{
	friend class FrameWork;
	friend class FrameWork2;
	friend class IService;
	friend class Video;

	ServiceManager();

	~ServiceManager();
public:
	static ServiceManager& GetInstance();
private:
	bool Initialize();

	bool Process();

	bool Pause();

	void Resume();

	void Terminate();

	void Add(IService* service);

	void Remove(IService* service);
private:
	std::vector<IService*> services;
};

#endif