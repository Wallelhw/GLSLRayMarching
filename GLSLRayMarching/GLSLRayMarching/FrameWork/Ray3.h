//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Ray3_h_
#define _Ray3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Vector3.h"

	class Ray3
	{
	public:
		// construction
		Ray3();  // uninitialized
		Ray3(const Vector3& origin_, const Vector3& direction_);

		void Read(InputStream& is)
		{
			is >> origin >> direction;
		}

		void Write(OutputStream& os) const
		{
			os << origin << direction;
		}

		Vector3 origin;
		Vector3 direction;
	};

#endif