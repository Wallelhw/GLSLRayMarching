//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Scene_h_
#define _Scene_h_

#include "Platform.h"

#include "String.h"

class Scene
{
public:
	Scene();

	virtual ~Scene();

	bool Initiate();

	bool Start();

	bool Update();

	bool Pause();

	void Stop();

	void Resume();

	void Terminate();
protected:
	virtual bool OnInitiate() = 0;

	virtual bool OnStart() = 0;

	virtual bool OnUpdate() = 0;

	virtual bool OnPause() = 0;

	virtual void OnResume() = 0;

	virtual void OnStop() = 0;

	virtual void OnTerminate() = 0;
protected:
	bool Push(const char* name_);
	bool Goto(const char* name_);
	bool Pop();

	const std::string& GetCurrentSceneName();

	/////////////////////////////////////////////////////////////////////
public:
	#define NAME_MAX 1024
	#define MAX_NUM_SCENES 1024
	class ICreator
	{
	public:
		ICreator(const char* name_);
		virtual ~ICreator();
		virtual Scene* Initiate() = 0;

		const std::string& GetName() const;
	private:
		std::string name;
	};

public:
	template<typename T> 
	class Creator : public ICreator
	{
	public:
		Creator(const char* name_)
		: ICreator(name_)
		{
		}

		virtual ~Creator()
		{
		}

		virtual Scene* Initiate()
		{
			return new T();
		}
	};

public:
	class Service;

	class Manager
	{
		friend class Scene;
		friend class ICreator;
		friend class Service;
		Manager();
		~Manager();
	public:
		static Scene::Manager& GetInstance();

	private:
		bool Initialize();
		bool Update();
		bool Pause();
		void Resume();
		void Terminate();
	private:
		Scene::ICreator* Find(const char* name_) const;
		bool Push(const char* name_);
		bool Goto(const char* name_);
		bool Pop();
		const std::string& GetCurrentSceneName() const;
	private:
		void Add(ICreator* iCreator);
		void Remove(ICreator* iCreator);
	private:
		std::vector<ICreator*> sceneCreators;
	private:
		std::vector<ICreator*> sceneStack;
		ICreator* currentCreator;
		ICreator* nextCreator;

		Scene* currentScene;
	};

	///////////////////////////////////////////////////////////////////////
public:
	class Service
	{
	public:
		static bool Initialize();
		static bool Update();
		static bool Pause();
		static void Resume();
		static void Terminate();
	};
};

#endif