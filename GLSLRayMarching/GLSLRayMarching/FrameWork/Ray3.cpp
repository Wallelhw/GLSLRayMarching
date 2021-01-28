//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Ray3.h"

    Ray3::Ray3()
    {
        // uninitialized
    }

    Ray3::Ray3(const Vector3& origin_, const Vector3& direction_)
        : origin(origin_)
        , direction(direction_)
    {
    }