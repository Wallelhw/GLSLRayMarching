//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _AABB3_h_
#define _AABB3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Vector3.h"

	class AABB3
	{
	public:
		AABB3();
		AABB3(const Vector3& min_, const Vector3& max_);
		~AABB3();

		AABB3(const AABB3& other);
		AABB3& operator = (const AABB3& other);

		void Init(const Vector3& min_, const Vector3& max_);
		Vector3& Min();
		Vector3& Max();
		const Vector3& Min() const;
		const Vector3& Max() const;
		Vector3 Center() const;
		Vector3 Extent() const;

		AABB3& operator+= (const AABB3& other);

		friend AABB3 operator + (const AABB3& aabb0, const AABB3& aabb1)
		{
			return AABB3(Vector3::Min(aabb0.min, aabb1.min), Vector3::Max(aabb0.max, aabb1.max));
		}

		void Read(InputStream& is)
		{
			is >> min >> max;
		}

		void Write(OutputStream& os) const
		{
			os << min << max;
		}
	protected:
	private:

	public:
	protected:
	private:
		Vector3 min;
		Vector3 max;
	};

#endif