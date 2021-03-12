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
#include "Vector2.h"

class Video
{
public:
	class Graphics3Component : public Component
	{
		friend class Video;
	public:
		Graphics3Component(GameObject& gameObject_);

		virtual ~Graphics3Component();
	private:
		void Render();

		virtual void OnRender() = 0;

		virtual bool OnConstruct() override;

		virtual bool OnStart() override;

		virtual bool OnUpdate() override;

		virtual bool OnPause() override;

		virtual void OnResume() override;

		virtual void OnStop() override;

		virtual void OnDestruct() override;
	};

	class RendererComponent : public Component
	{
		friend class Video;
	public:
		RendererComponent(GameObject& gameObject_);

		virtual ~RendererComponent();
	
	private:
		void Render();

		virtual void OnRender() = 0;

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
		friend class Video;
	public:
		CameraComponent(GameObject& gameObject_);

		virtual ~CameraComponent();
	private:
		void Render();

		virtual void OnRender() = 0;

		virtual bool OnConstruct() override;

		virtual bool OnStart() override;

		virtual bool OnUpdate() override;

		virtual bool OnPause() override;

		virtual void OnResume() override;

		virtual void OnStop() override;

		virtual void OnDestruct() override;
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

		bool IsKeyPressed(char key) const;
		bool IsKeyRelease(char key) const;
		bool IsKeyRepeat(char key) const;
		void GetTheta(float& theta_, float& phi_) const;
		double GetTime() const;
		double GetDeltaTime() const;
		Vector4 GetMouse() const;
		Vector2 GetMouseDelta() const;
		int GetMouseLeftButtonStatus() const;
		int GetMouseMidButtonStatus() const;
		int GetMouseRightButtonStatus() const;
		int GetFrameCounter() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		bool ShouldClose() const;
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