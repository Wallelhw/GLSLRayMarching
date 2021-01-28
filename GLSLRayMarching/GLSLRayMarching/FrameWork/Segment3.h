//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Segment3_h_
#define _Segment3_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Vector3.h"

	class Segment3
	{
	public:
		// construction
		Segment3();  // uninitialized
		Segment3(const Vector3& rkOrigin, const Vector3& rkDirection, float fExtent);

		// end points
		Vector3 GetPosEnd() const;  // P+e*D
		Vector3 GetNegEnd() const;  // P-e*D

		void Read(InputStream& is)
		{
			is >> Origin >> Direction >> Extent;
		}

		void Write(OutputStream& os) const
		{
			os << Origin << Direction << Extent;
		}

		Vector3 Origin, Direction;
		float Extent;
	};

#endif