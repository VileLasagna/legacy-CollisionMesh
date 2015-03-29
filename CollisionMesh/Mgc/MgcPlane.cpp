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

#include "MgcPlane.h"

//----------------------------------------------------------------------------
MgcPlane::MgcPlane ()
{
    m_kNormal = MgcVector3::ZERO;
    m_fConstant = 0.0;
}
//----------------------------------------------------------------------------
MgcPlane::MgcPlane (const MgcVector3& rkNormal, MgcReal fConstant)
{
    m_kNormal = rkNormal;
    m_fConstant = fConstant;
}
//----------------------------------------------------------------------------
MgcPlane::MgcPlane (const MgcVector3& rkNormal, const MgcVector3& rkPoint)
{
    m_kNormal = rkNormal;
    m_fConstant = rkNormal.Dot(rkPoint);
}
//----------------------------------------------------------------------------
MgcPlane::MgcPlane (const MgcVector3& rkPoint0, const MgcVector3& rkPoint1,
    const MgcVector3& rkPoint2)
{
    MgcVector3 kEdge1 = rkPoint1 - rkPoint0;
    MgcVector3 kEdge2 = rkPoint2 - rkPoint0;
    m_kNormal = kEdge1.UnitCross(kEdge2);
    m_fConstant = m_kNormal.Dot(rkPoint0);
}
//----------------------------------------------------------------------------
MgcReal MgcPlane::DistanceTo (const MgcVector3& rkPoint) const
{
    return m_kNormal.Dot(rkPoint) - m_fConstant;
}
//----------------------------------------------------------------------------
MgcPlane::Side MgcPlane::WhichSide (const MgcVector3& rkPoint) const
{
    MgcReal fDistance = DistanceTo(rkPoint);

    if ( fDistance < 0.0 )
        return MgcPlane::NEGATIVE_SIDE;

    if ( fDistance > 0.0 )
        return MgcPlane::POSITIVE_SIDE;

    return MgcPlane::NO_SIDE;
}
//----------------------------------------------------------------------------
