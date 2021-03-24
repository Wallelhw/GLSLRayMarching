#ifndef _PathTrace_h_
#define _PathTrace_h_

class PathTraceImp;

class PathTrace
{
public:
	PathTrace();
	~PathTrace();

	bool Initiate(const char* path_);
	bool Start();
	bool Update();
	bool Pause();
	bool Resume();
	bool Stop();
	void Terminate();
	void Render();
private:
	PathTraceImp* impl;
};

#endif