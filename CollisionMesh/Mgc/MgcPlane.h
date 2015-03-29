// Magic Software, Inc.
// http://www.magic-software.com
// Copyright (c) 2000, All Rights Reserved
//
// Source code from Magic Software is supplied under the terms of a license
// agreement and may not be copied or disclosed except in accordance with the
// terms of that agreement.  The various license agreements may be found at
// the Magic Software web site.  This file is subject to the license
//
// FREE SOURCE CODE
// http://www.magic-software.com/License/free.pdf

#ifndef MGCPLANE_H
#define MGCPLANE_H

#include "MgcVector3.h"


class MgcPlane
{
public:
    // construction
    MgcPlane ();
    MgcPlane (const MgcVector3& rkNormal, MgcReal fConstant);
    MgcPlane (const MgcVector3& rkNormal, const MgcVector3& rkPoint);
    MgcPlane (const MgcVector3& rkPoint0, const MgcVector3& rkPoint1,
        const MgcVector3& rkPoint2);

    MgcVector3& Normal ();
    const MgcVector3& Normal () const;

    MgcReal& Constant ();
    const MgcReal& Constant () const;

    // The "positive side" of the plane is the half space to which the plane
    // normal points.  The "negative side" is the other half space.  The flag
    // "no side" indicates the plane itself.
    enum Side
    {
        NO_SIDE,
        POSITIVE_SIDE,
        NEGATIVE_SIDE
    };

    Side WhichSide (const MgcVector3& rkPoint) const;

    // This is a pseudodistance.  The sign of the return value is positive if
    // the point is on the positive side of the plane, negative if the point
    // is on the negative side, and zero if the point is on the plane.  The
    // absolute value of the return value is the true distance only when the
    // plane normal is a unit length vector.
    MgcReal DistanceTo (const MgcVector3& rkPoint) const;

protected:
    // plane is Dot(m_normal,(x,y,z)) = m_fConstant
    MgcVector3 m_kNormal;
    MgcReal m_fConstant;
};

#include "MgcPlane.inl"

#endif
