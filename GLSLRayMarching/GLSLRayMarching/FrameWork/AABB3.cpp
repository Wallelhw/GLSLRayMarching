//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "AABB3.h"

	AABB3::AABB3()
		: min(Vector3::Zero)
		, max(Vector3::Zero)
	{
	}

	AABB3::AABB3(const Vector3& min_, const Vector3& max_)
	{
		Init(min_, max_);
	}

	AABB3::~AABB3()
	{
	}

	AABB3::AABB3(const AABB3& other)
		: min(other.min)
		, max(other.max)
	{
	}

	AABB3& AABB3::operator = (const AABB3& other)
	{
		this->min = other.min;
		this->max = other.max;

		return *this;
	}

	void AABB3::Init(const Vector3& min_, const Vector3& max_)
	{
		min = Vector3::Min(min_, max_);
		max = Vector3::Max(min_, max_);
	}

	Vector3& AABB3::Min()
	{
		return min;
	}

	Vector3& AABB3::Max()
	{
		return max;
	}

	const Vector3& AABB3::Min() const
	{
		return min;
	}

	const Vector3& AABB3::Max() const
	{
		return max;
	}

	Vector3 AABB3::Center() const
	{
		return (max + min) / 2.0;
	}

	Vector3 AABB3::Extent() const
	{
		return Vector3::FAbs(max - min) / 2.0;
	}

	AABB3& AABB3::operator+= (const AABB3& other)
	{
		min = Vector3::Min(min, other.min);
		max = Vector3::Max(max, other.max);

		return *this;
	}
