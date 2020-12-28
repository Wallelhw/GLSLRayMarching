#ifndef _IMath_h_
#define _IMath_h_

#include "assert.h"

class Math
{
public:
	static float Abs(float v);
	static float Sqrt(float v);
	static float Sqr(float v);

	static float Sin(float radian);
	static float Cos(float radian);
	static float Tan(float radian);
	static float ASin(float value);
	static float ACos(float value);
	static float ATan(float value);
	static float ATan2(float y, float x);

	static float Min(float v0, float v1);
	static float Max(float v0, float v1);

	static float HalfPi;
	static float OnePi;
	static float TwoPi;

	static float Deg2Rad;
	static float Rad2Deg;
	static float ZeroTolerance;
	static float Epison;
};

#endif