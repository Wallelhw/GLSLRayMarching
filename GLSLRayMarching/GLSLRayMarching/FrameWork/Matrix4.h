#ifndef _Matrix4_h_
#define _Matrix4_h_

#include "Math.h"

template<class T>
class Matrix4
{
public:
	Matrix4()
	{
		m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
		m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
		m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
		m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
	}

	~Matrix4()
	{
	}

	float* operator [] (int row)
	{
		assert(row >= 0 && row < 4);
		return m[row];
	}

	const float* operator [] (int row) const
	{
		assert(row >= 0 && row < 4);
		return m[row];
	}

	Matrix4 Inverse()
	{
		return Matrix4();
	}
private:
	T m[4][4];
};

typedef Matrix4<unsigned int> umat4;
typedef Matrix4<int> imat4;
typedef Matrix4<float> mat4;
typedef Matrix4<double> dmat4;

#endif