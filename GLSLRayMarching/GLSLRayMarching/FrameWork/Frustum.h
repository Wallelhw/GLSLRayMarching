//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Frustum_h_
#define _Frustum_h_

#include "Platform.h"
#include "Matrix4.h"
#include "Plane3.h"
#include "InputStream.h"
#include "OutputStream.h"

class Frustum
{
public:
	Frustum();
	Frustum(const Matrix4& transform_, float l_, float r_, float b, float t_, float n_, float f_);
	~Frustum();
	Frustum(const Frustum& other);
	Frustum& operator = (const Frustum& other);

	const Plane3& GetPlane(int i) const
	{
		return planes[i];
	}

	int GetPlaneCount() const
	{
		return 6;
	}

	void Read(InputStream& is);
	void Write(OutputStream& os) const;
protected:
	void Build(const Matrix4& transform_, float l_, float r_, float b_, float t_, float n_, float f_);
private:

public:
protected:
private:
	Plane3 planes[6];
};

#endif