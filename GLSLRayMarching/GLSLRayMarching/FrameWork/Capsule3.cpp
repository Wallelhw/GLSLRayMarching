//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Capsule3.h"

    Capsule3::Capsule3()
    {
    }

    Capsule3::Capsule3(const Segment3& segment_, float radius_)
        : segment(segment_)
        , radius(radius_)
    {
    }
