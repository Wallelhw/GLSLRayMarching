//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Video_h_
#define _Video_h_

#include "Platform.h" 
#include "Component.h"

class Video
{
public:
	class Graphics3Component : public Component
	{
	public:
		Graphics3Component(Component::Owner& owner_);

		virtual ~Graphics3Component();
	
		virtual bool OnConstruct() override;

		virtual bool OnStart() override;

		virtual bool OnUpdate() override;

		virtual bool OnPause() override;

		virtual void OnResume() override;

		virtual void OnStop() override;

		virtual void OnDestruct() override;
	private:
	};

	class RendererComponent : public Component
	{
	public:
		RendererComponent(Component::Owner& owner_);

		virtual ~RendererComponent();

		virtual bool OnConstruct() override;

		virtual bool OnStart() override;

		virtual bool OnUpdate() override;

		virtual bool OnPause() override;

		virtual void OnResume() override;

		virtual void OnStop() override;

		virtual void OnDestruct() override;
	private:
	};

	class CameraComponent : public Component
	{
	public:
		CameraComponent(Component::Owner& owner_);

		virtual ~CameraComponent();

		virtual bool OnConstruct() override;

		virtual bool OnStart() override;

		virtual bool OnUpdate() override;

		virtual bool OnPause() override;

		virtual void OnResume() override;

		virtual void OnStop() override;

		virtual void OnDestruct() override;
	private:
	};

	class Manager
	{
		Manager();
		~Manager();
	public:
		static Video::Manager& GetInstance();

		bool Initialize();
		bool Process();
		bool Pause();
		void Resume();
		void Terminate();
	
		void Add(CameraComponent* camera);
		void Add(RendererComponent* renderer);
		void Add(Graphics3Component* graphics3);
		void Remove(CameraComponent* camera);
		void Remove(RendererComponent* renderer);
		void Remove(Graphics3Component* graphics3);
	private:
		std::vector<CameraComponent*> cameras;
		std::vector<RendererComponent*> renderers;
		std::vector<Graphics3Component*> graphics3s;
	};

	///////////////////////////////////////////////////////////////////////
public:
	class Service
	{
	public:
		static bool Initialize();
		static bool Process();
		static bool Pause();
		static void Resume();
		static void Terminate();
	};
};

#endif