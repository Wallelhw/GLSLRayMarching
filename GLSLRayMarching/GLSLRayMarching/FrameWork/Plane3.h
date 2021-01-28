//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Plane3_h_
#define _Plane3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Vector3.h"

	class Plane3
	{
	public:
		Plane3();  // uninitialized
		Plane3(const Plane3& plane);

		Plane3(const Vector3& normal, float constant);

		Plane3(const Vector3& normal, const Vector3& p);

		Plane3(const Vector3& p0, const Vector3& p1, const Vector3& p2);

		Plane3& operator= (const Plane3& plane);

		const Vector3& Normal() const
		{
			return normal;
		}

		float Constant() const
		{
			return constant;
		}

		int WhichSide(const Vector3& p) const;

		float DistanceTo(const Vector3& q) const;

		bool IsCoplanar(const Plane3& plane) const;

		void Flip()
		{
			normal = -normal;
			constant = -constant;
		}

		void Read(InputStream& is)
		{
			is >> normal >> constant;
		}

		void Write(OutputStream& os) const
		{
			os << normal << constant;
		}
	private:
		Vector3 normal;
		float constant;
	};

#endif