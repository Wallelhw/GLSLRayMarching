//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _GameObject_h_
#define _GameObject_h_

#include "Platform.h"
#include "Component.h"

#include <rttr/type>

enum class color
{
	red,
	green,
	blue
};

class point2d
{
public:
	point2d() {}
	point2d(int x_, int y_) : x(x_), y(y_) {}
	int x = 0;
	int y = 0;
};

class GameObject : public Component::Owner
{
public:
	class Manager
	{
		friend class Component;
		friend class GameObject;
		friend class Scene;
		Manager();
		~Manager();
	public:
		static Manager& GetInstance();
	private:
		bool Construct();

		bool Start();

		bool Update();

		bool Pause();

		void Resume();

		void Stop();

		void Destruct();

		void Add(GameObject* service);

		void Remove(GameObject* service);
	private:
		std::vector<GameObject*> gameObjects;
	};
///////////////////////////////////////////////////////////////
public:
	GameObject();

	GameObject(int test);
	
	virtual ~GameObject();
private:
	virtual bool OnConstruct();

	virtual bool OnStart();

	virtual bool OnUpdate();

	virtual bool OnPause();

	virtual void OnResume();

	virtual void OnStop();

	virtual void OnDestruct();
protected:
private:
///////////////////////////////////////////////////////////////
public:
	void set_visible(bool v) { visible = v; }
	bool get_visible() const { return visible; }

	color color1 = color::blue;
	std::string name = "";
	point2d position;
	std::map<color, point2d> dictionary;

	RTTR_ENABLE(Component::Owner)
private:
	bool visible = false;
};

#endif