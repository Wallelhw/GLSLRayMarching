//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Plane3.h"

    Plane3::Plane3()
    {
        // uninitialized
    }

    Plane3::Plane3(const Plane3& plane_)
    {
        normal = plane_.normal;
        constant = plane_.constant;
    }

    Plane3::Plane3(const Vector3& normal_, float constant_)
    {
        normal = normal_;
        constant = constant_;
    }

    Plane3::Plane3(const Vector3& normal_, const Vector3& p_)
    {
        normal = normal_;
        constant = normal.Dot(p_);
    }

    Plane3::Plane3(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        normal = (b - a).UnitCross(c - a);
        constant = normal.Dot(a);
    }

    Plane3& Plane3::operator = (const Plane3& plane_)
    {
        this->normal = plane_.normal;
        this->constant = plane_.constant;

        return *this;
    }

    float Plane3::DistanceTo(const Vector3& p) const
    {
        return normal.Dot(p) - constant;
    }

    bool Plane3::IsCoplanar(const Plane3& plane) const
    {
        return
            Math::FAbs(normal[0] - plane.normal[0]) < Math::Epsilon &&
            Math::FAbs(normal[1] - plane.normal[1]) < Math::Epsilon &&
            Math::FAbs(normal[2] - plane.normal[2]) < Math::Epsilon &&
            Math::FAbs(constant - plane.constant) < Math::Epsilon;
    }

    /*
    inline void Plane3::read(InputStream& is)
    {
        is >> Normal >> Constant;
    }


    inline void Plane3::write(OutputStream& os) const
    {
        os << Normal << Constant;
    }
    */

    int Plane3::WhichSide(const Vector3& q) const
    {
        float distance = DistanceTo(q);

        if (distance > 0.01f)
            return +1;
        else if (distance < -0.01f)
            return -1;
        else
            return 0;
    }