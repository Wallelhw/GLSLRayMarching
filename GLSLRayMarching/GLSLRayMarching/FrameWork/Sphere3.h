//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Sphere3_h_
#define _Sphere3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"

	class Sphere3
	{
	public:
		// construction
		Sphere3();  // uninitialized
		Sphere3(float fRadius);

		void Read(InputStream& is)
		{
			is >> radius;
		}

		void Write(OutputStream& os) const
		{
			os << radius;
		}

		float radius;
	};

#endif