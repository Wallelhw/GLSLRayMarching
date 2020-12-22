#include "IMath.h"
#include <algorithm>
#include <limits>

float Math::HalfPi = Math::OnePi * 0.5f;
float Math::OnePi = 3.141592654;
float Math::TwoPi = Math::OnePi * 2.0f;

float Math::Deg2Rad = (Math::OnePi / 180.f);
float Math::Rad2Deg = (180.f / Math::OnePi);

float Math::Sin(float radian)
{
	return sin(radian);
}

float Math::Cos(float radian)
{
	return cos(radian);
}

float Math::Tan(float radian)
{
	return tan(radian);
}

float Math::ASin(float value)
{
	return asin(value);
}

float Math::ACos(float value)
{
	return acos(value);
}

float Math::ATan(float value)
{
	return atan(value);
}

float Math::ATan2(float y, float x)
{
	return atan2(y, x);
}

float Math::Min(float v0, float v1)
{
	return std::min(v0, v1);
}

float Math::Max(float v0, float v1)
{
	return std::max(v0, v1);
}