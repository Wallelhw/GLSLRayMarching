#ifndef _FrameWork_h_
#define _FrameWork_h_

#include "Platform.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class FrameWorkImpl;

class FrameWork
{
public:
	FrameWork(const std::string& name_);
	virtual ~FrameWork();

	bool Create(int width_, int height_);
	bool Start();
	void Destroy();

	static FrameWork& GetInstance();

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

protected:
	virtual bool OnCreate() = 0;
	virtual bool OnUpdate() = 0;
	virtual void OnDestroy() = 0;
private:
	static void framebuffer_size_callback(void* window, int width, int height);
	static void processInput(void* window);
private:
	static FrameWork* instance;

public:
protected:
private:
	FrameWorkImpl* impl;
};

#endif