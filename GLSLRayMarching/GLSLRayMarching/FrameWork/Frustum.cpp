//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::Frustum(const Matrix4& transform_, float l_, float r_, float b_, float t_, float n_, float f_)
{
	Build(transform_, l_, r_, b_, t_, n_, f_);
}

Frustum::~Frustum()
{
}

Frustum::Frustum(const Frustum& other)
{
	for (int i = 0; i < 6; i++)
		planes[i] = other.planes[i];
}

Frustum& Frustum::operator = (const Frustum& other)
{
	for (int i = 0; i < 6; i++)
		planes[i] = other.planes[i];

	return *this;
}

void Frustum::Build(const Matrix4& transform_, float l_, float r_, float b_, float t_, float n_, float f_)
{
	planes[0] = Plane3(transform_ * Vector3(0, 0, 0), transform_ * Vector3(l_, t_, -n_), transform_ * Vector3(l_, b_, -n_));
	planes[1] = Plane3(transform_ * Vector3(0, 0, 0), transform_ * Vector3(r_, b_, -n_), transform_ * Vector3(r_, t_, -n_));
	planes[2] = Plane3(transform_ * Vector3(0, 0, 0), transform_ * Vector3(r_, t_, -n_), transform_ * Vector3(l_, t_, -n_));
	planes[3] = Plane3(transform_ * Vector3(0, 0, 0), transform_ * Vector3(l_, b_, -n_), transform_ * Vector3(r_, b_, -n_));
	planes[4] = Plane3(transform_ * Vector3(r_, b_, -n_), transform_ * Vector3(r_, t_, -n_), transform_ * Vector3(l_, t_, -n_));
	planes[5] = Plane3(transform_ * Vector3(r_, b_, -f_), transform_ * Vector3(l_, t_, -f_), transform_ * Vector3(r_, t_, -f_));
}

void Frustum::Read(InputStream& is)
{
	for (int i = 0; i < 6; i++)
		is >> planes[i];
}

void Frustum::Write(OutputStream& os) const
{
	for (int i = 0; i < 6; i++)
		os << planes[i];
}