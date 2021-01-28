//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Segment3.h"

    Segment3::Segment3()
    {
        // uninitialized
    }

    Segment3::Segment3(const Vector3& rkOrigin,
        const Vector3& rkDirection, float fExtent)
        :
        Origin(rkOrigin),
        Direction(rkDirection),
        Extent(fExtent)
    {
    }

    Vector3 Segment3::GetPosEnd() const
    {
        return Origin + Extent * Direction;
    }

    Vector3 Segment3::GetNegEnd() const
    {
        return Origin - Extent * Direction;
    }
