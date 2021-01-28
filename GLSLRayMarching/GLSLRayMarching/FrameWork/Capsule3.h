//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Capsule3_h_
#define _Capsule3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Segment3.h"

	class Capsule3
	{
	public:
		// construction
		Capsule3();  // uninitialized
		Capsule3(const Segment3& segment_, float radius_);

		void Read(InputStream& is)
		{
			is >> segment >> radius;
		}

		void Write(OutputStream& os) const
		{
			os << segment << radius;
		}

		Segment3 segment;
		float radius;
	};

#endif