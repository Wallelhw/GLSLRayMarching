#ifndef _PathTrace_h_
#define _PathTrace_h_

class PathTraceImp;

class PathTrace
{
public:
	PathTrace();
	~PathTrace();

	bool Construct(const char* path_);
	void Render();
	void Destroy();
private:
	PathTraceImp* impl;
};

#endif