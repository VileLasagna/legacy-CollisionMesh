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

#include "MgcDistLin3Lin3.h"
#include "MgcDistVec3Tri3.h"
#include "MgcDistLin3Tri3.h"
#include "MgcRTLib.h"

static const MgcReal gs_fTolerance = 1e-05;

//----------------------------------------------------------------------------
MgcReal MgcSqrDistance (const MgcLine3& rkLine, const MgcTriangle3& rkTri,
    MgcReal* pfLinP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    MgcVector3 kDiff = rkTri.Origin() - rkLine.Origin();
    MgcReal fA00 = rkLine.Direction().SquaredLength();
    MgcReal fA01 = -rkLine.Direction().Dot(rkTri.Edge0());
    MgcReal fA02 = -rkLine.Direction().Dot(rkTri.Edge1());
    MgcReal fA11 = rkTri.Edge0().SquaredLength();
    MgcReal fA12 = rkTri.Edge0().Dot(rkTri.Edge1());
    MgcReal fA22 = rkTri.Edge1().Dot(rkTri.Edge1());
    MgcReal fB0  = -kDiff.Dot(rkLine.Direction());
    MgcReal fB1  = kDiff.Dot(rkTri.Edge0());
    MgcReal fB2  = kDiff.Dot(rkTri.Edge1());
    MgcReal fCof00 = fA11*fA22-fA12*fA12;
    MgcReal fCof01 = fA02*fA12-fA01*fA22;
    MgcReal fCof02 = fA01*fA12-fA02*fA11;
    MgcReal fDet = fA00*fCof00+fA01*fCof01+fA02*fCof02;

    MgcSegment3 kTriSeg;
    MgcVector3 kPt;
    MgcReal fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

    if ( MgcMath::Abs(fDet) >= gs_fTolerance )
    {
        MgcReal fCof11 = fA00*fA22-fA02*fA02;
        MgcReal fCof12 = fA02*fA01-fA00*fA12;
        MgcReal fCof22 = fA00*fA11-fA01*fA01;
        MgcReal fInvDet = 1.0/fDet;
        MgcReal fRhs0 = -fB0*fInvDet;
        MgcReal fRhs1 = -fB1*fInvDet;
        MgcReal fRhs2 = -fB2*fInvDet;

        fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
        fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

        if ( fS+fT <= 1.0 )
        {
            if ( fS < 0.0 )
            {
                if ( fT < 0.0 )  // region 4
                {
                    // min on face s=0 or t=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkLine,kTriSeg,0,&fS0);
                    fT0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 3
                {
                    // min on face s=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fT);
                    fS = 0.0;
                }
            }
            else if ( fT < 0.0 )  // region 5
            {
                // min on face t=0
                kTriSeg.Origin() = rkTri.Origin();
                kTriSeg.Direction() = rkTri.Edge0();
                fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fS);
                fT = 0.0;
            }
            else  // region 0
            {
                // line intersects triangle
                fSqrDist = 0.0;
            }
        }
        else
        {
            if ( fS < 0.0 )  // region 2
            {
                // min on face s=0 or s+t=1
                kTriSeg.Origin() = rkTri.Origin();
                kTriSeg.Direction() = rkTri.Edge1();
                fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fT);
                fS = 0.0;
                kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                fSqrDist0 = MgcSqrDistance(rkLine,kTriSeg,0,&fT0);
                fS0 = 1.0-fT0;
                if ( fSqrDist0 < fSqrDist )
                {
                    fSqrDist = fSqrDist0;
                    fS = fS0;
                    fT = fT0;
                }
            }
            else if ( fT < 0.0 )  // region 6
            {
                // min on face t=0 or s+t=1
                kTriSeg.Origin() = rkTri.Origin();
                kTriSeg.Direction() = rkTri.Edge0();
                fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fS);
                fT = 0.0;
                kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                fSqrDist0 = MgcSqrDistance(rkLine,kTriSeg,0,&fT0);
                fS0 = 1.0-fT0;
                if ( fSqrDist0 < fSqrDist )
                {
                    fSqrDist = fSqrDist0;
                    fS = fS0;
                    fT = fT0;
                }
            }
            else  // region 1
            {
                // min on face s+t=1
                kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                fSqrDist = MgcSqrDistance(rkLine,kTriSeg,0,&fT);
                fS = 1.0-fT;
            }
        }
    }
    else
    {
        // line and triangle are parallel
        kTriSeg.Origin() = rkTri.Origin();
        kTriSeg.Direction() = rkTri.Edge0();
        fSqrDist = MgcSqrDistance(rkLine,kTriSeg,&fR,&fS);
        fT = 0.0;

        kTriSeg.Direction() = rkTri.Edge1();
        fSqrDist0 = MgcSqrDistance(rkLine,kTriSeg,&fR0,&fT0);
        fS0 = 0.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        kTriSeg.Origin() = rkTri.Origin() + rkTri.Edge0();
        kTriSeg.Direction() = rkTri.Edge1() - rkTri.Edge0();
        fSqrDist0 = MgcSqrDistance(rkLine,kTriSeg,&fR0,&fT0);
        fS0 = 1.0-fT0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }
    }

    if ( pfLinP )
        *pfLinP = fR;

    if ( pfTriP0 )
        *pfTriP0 = fS;

    if ( pfTriP1 )
        *pfTriP1 = fT;

    return MgcMath::Abs(fSqrDist);
}
//----------------------------------------------------------------------------
MgcReal MgcSqrDistance (const MgcRay3& rkRay, const MgcTriangle3& rkTri,
    MgcReal* pfRayP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    MgcVector3 kDiff = rkTri.Origin() - rkRay.Origin();
    MgcReal fA00 = rkRay.Direction().SquaredLength();
    MgcReal fA01 = -rkRay.Direction().Dot(rkTri.Edge0());
    MgcReal fA02 = -rkRay.Direction().Dot(rkTri.Edge1());
    MgcReal fA11 = rkTri.Edge0().SquaredLength();
    MgcReal fA12 = rkTri.Edge0().Dot(rkTri.Edge1());
    MgcReal fA22 = rkTri.Edge1().Dot(rkTri.Edge1());
    MgcReal fB0  = -kDiff.Dot(rkRay.Direction());
    MgcReal fB1  = kDiff.Dot(rkTri.Edge0());
    MgcReal fB2  = kDiff.Dot(rkTri.Edge1());
    MgcReal fCof00 = fA11*fA22-fA12*fA12;
    MgcReal fCof01 = fA02*fA12-fA01*fA22;
    MgcReal fCof02 = fA01*fA12-fA02*fA11;
    MgcReal fDet = fA00*fCof00+fA01*fCof01+fA02*fCof02;

    MgcSegment3 kTriSeg;
    MgcVector3 kPt;
    MgcReal fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

    if ( MgcMath::Abs(fDet) >= gs_fTolerance )
    {
        MgcReal fCof11 = fA00*fA22-fA02*fA02;
        MgcReal fCof12 = fA02*fA01-fA00*fA12;
        MgcReal fCof22 = fA00*fA11-fA01*fA01;
        MgcReal fInvDet = 1.0/fDet;
        MgcReal fRhs0 = -fB0*fInvDet;
        MgcReal fRhs1 = -fB1*fInvDet;
        MgcReal fRhs2 = -fB2*fInvDet;

        fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
        fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

        if ( fR <= 0.0 )
        {
            if ( fS+fT <= 1.0 )
            {
                if ( fS < 0.0 )
                {
                    if ( fT < 0.0 )  // region 4m
                    {
                        // min on face s=0 or t=0 or r=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge0();
                        fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,&fR0,&fS0);
                        fT0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                        fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,
                            &fS0,&fT0);
                        fR0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3m
                    {
                        // min on face s=0 or r=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,
                            &fS0,&fT0);
                        fR0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                }
                else if ( fT < 0.0 )  // region 5m
                {
                    // min on face t=0 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 0m
                {
                    // min on face r=0
                    fSqrDist = MgcSqrDistance(rkRay.Origin(),rkTri,&fS,&fT);
                    fR = 0.0;
                }
            }
            else
            {
                if ( fS < 0.0 )  // region 2m
                {
                    // min on face s=0 or s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0 )  // region 6m
                {
                    // min on face t=0 or s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1m
                {
                    // min on face s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fT);
                    fS = 1.0-fT;
                    fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
            }
        }
        else  // fR > 0
        {
            if ( fS+fT <= 1.0 )
            {
                if ( fS < 0.0 )
                {
                    if ( fT < 0.0 )  // region 4p
                    {
                        // min on face s=0 or t=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fT);
                        fS = 0.0;
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge0();
                        fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,0,&fS0);
                        fT0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3p
                    {
                        // min on face s=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fT);
                        fS = 0.0;
                    }
                }
                else if ( fT < 0.0 )  // region 5p
                {
                    // min on face t=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fS);
                    fT = 0.0;
                }
                else  // region 0p
                {
                    // ray intersects triangle
                    fSqrDist = 0.0;
                }
            }
            else
            {
                if ( fS < 0.0 )  // region 2p
                {
                    // min on face s=0 or s+t=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0 )  // region 6p
                {
                    // min on face t=0 or s+t=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fS);
                    fT = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1p
                {
                    // min on face s+t=1
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkRay,kTriSeg,0,&fT);
                    fS = 1.0-fT;
                }
            }
        }
    }
    else
    {
        // ray and triangle are parallel
        kTriSeg.Origin() = rkTri.Origin();
        kTriSeg.Direction() = rkTri.Edge0();
        fSqrDist = MgcSqrDistance(rkRay,kTriSeg,&fR,&fS);
        fT = 0.0;

        kTriSeg.Direction() = rkTri.Edge1();
        fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,&fR0,&fT0);
        fS0 = 0.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        kTriSeg.Origin() = rkTri.Origin() + rkTri.Edge0();
        kTriSeg.Direction() = rkTri.Edge1() - rkTri.Edge0();
        fSqrDist0 = MgcSqrDistance(rkRay,kTriSeg,&fR0,&fT0);
        fS0 = 1.0-fT0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        fSqrDist0 = MgcSqrDistance(rkRay.Origin(),rkTri,&fS0,&fT0);
        fR0 = 0.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }
    }

    if ( pfRayP )
        *pfRayP = fR;

    if ( pfTriP0 )
        *pfTriP0 = fS;

    if ( pfTriP1 )
        *pfTriP1 = fT;

    return MgcMath::Abs(fSqrDist);
}
//----------------------------------------------------------------------------
MgcReal MgcSqrDistance (const MgcSegment3& rkSeg, const MgcTriangle3& rkTri,
    MgcReal* pfSegP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    MgcVector3 kDiff = rkTri.Origin() - rkSeg.Origin();
    MgcReal fA00 = rkSeg.Direction().SquaredLength();
    MgcReal fA01 = -rkSeg.Direction().Dot(rkTri.Edge0());
    MgcReal fA02 = -rkSeg.Direction().Dot(rkTri.Edge1());
    MgcReal fA11 = rkTri.Edge0().SquaredLength();
    MgcReal fA12 = rkTri.Edge0().Dot(rkTri.Edge1());
    MgcReal fA22 = rkTri.Edge1().Dot(rkTri.Edge1());
    MgcReal fB0  = -kDiff.Dot(rkSeg.Direction());
    MgcReal fB1  = kDiff.Dot(rkTri.Edge0());
    MgcReal fB2  = kDiff.Dot(rkTri.Edge1());
    MgcReal fCof00 = fA11*fA22-fA12*fA12;
    MgcReal fCof01 = fA02*fA12-fA01*fA22;
    MgcReal fCof02 = fA01*fA12-fA02*fA11;
    MgcReal fDet = fA00*fCof00+fA01*fCof01+fA02*fCof02;

    MgcSegment3 kTriSeg;
    MgcVector3 kPt;
    MgcReal fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

    if ( MgcMath::Abs(fDet) >= gs_fTolerance )
    {
        MgcReal fCof11 = fA00*fA22-fA02*fA02;
        MgcReal fCof12 = fA02*fA01-fA00*fA12;
        MgcReal fCof22 = fA00*fA11-fA01*fA01;
        MgcReal fInvDet = 1.0/fDet;
        MgcReal fRhs0 = -fB0*fInvDet;
        MgcReal fRhs1 = -fB1*fInvDet;
        MgcReal fRhs2 = -fB2*fInvDet;

        fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
        fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

        if ( fR < 0.0 )
        {
            if ( fS+fT <= 1.0 )
            {
                if ( fS < 0.0 )
                {
                    if ( fT < 0.0 )  // region 4m
                    {
                        // min on face s=0 or t=0 or r=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge0();
                        fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fS0);
                        fT0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                        fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,
                            &fS0,&fT0);
                        fR0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3m
                    {
                        // min on face s=0 or r=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,
                            &fS0,&fT0);
                        fR0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                }
                else if ( fT < 0.0 )  // region 5m
                {
                    // min on face t=0 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 0m
                {
                    // min on face r=0
                    fSqrDist = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS,&fT);
                    fR = 0.0;
                }
            }
            else
            {
                if ( fS < 0.0 )  // region 2m
                {
                    // min on face s=0 or s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0 )  // region 6m
                {
                    // min on face t=0 or s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1m
                {
                    // min on face s+t=1 or r=0
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 1.0-fT;
                    fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS0,
                        &fT0);
                    fR0 = 0.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
            }
        }
        else if ( fR <= 1.0 )
        {
            if ( fS+fT <= 1.0 )
            {
                if ( fS < 0.0 )
                {
                    if ( fT < 0.0 )  // region 4
                    {
                        // min on face s=0 or t=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge0();
                        fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fS0);
                        fT0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3
                    {
                        // min on face s=0
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                    }
                }
                else if ( fT < 0.0 )  // region 5
                {
                    // min on face t=0
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                }
                else  // region 0
                {
                    // global minimum is interior, done
                    fSqrDist = fR*(fA00*fR+fA01*fS+fA02*fT+2.0*fB0)
                          +fS*(fA01*fR+fA11*fS+fA12*fT+2.0*fB1)
                          +fT*(fA02*fR+fA12*fS+fA22*fT+2.0*fB2)
                          +kDiff.SquaredLength();
                }
            }
            else
            {
                if ( fS < 0.0 )  // region 2
                {
                    // min on face s=0 or s+t=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0 )  // region 6
                {
                    // min on face t=0 or s+t=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1
                {
                    // min on face s+t=1
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 1.0-fT;
                }
            }
        }
        else  // fR > 1
        {
            if ( fS+fT <= 1.0 )
            {
                if ( fS < 0.0 )
                {
                    if ( fT < 0.0 )  // region 4p
                    {
                        // min on face s=0 or t=0 or r=1
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge0();
                        fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fS0);
                        fT0 = 0.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                        kPt = rkSeg.Origin()+rkSeg.Direction();
                        fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                        fR0 = 1.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3p
                    {
                        // min on face s=0 or r=1
                        kTriSeg.Origin() = rkTri.Origin();
                        kTriSeg.Direction() = rkTri.Edge1();
                        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                        fS = 0.0;
                        kPt = rkSeg.Origin()+rkSeg.Direction();
                        fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                        fR0 = 1.0;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                }
                else if ( fT < 0.0 )  // region 5p
                {
                    // min on face t=0 or r=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    kPt = rkSeg.Origin()+rkSeg.Direction();
                    fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                    fR0 = 1.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 0p
                {
                    // min face on r=1
                    kPt = rkSeg.Origin()+rkSeg.Direction();
                    fSqrDist = MgcSqrDistance(kPt,rkTri,&fS,&fT);
                    fR = 1.0;
                }
            }
            else
            {
                if ( fS < 0.0 )  // region 2p
                {
                    // min on face s=0 or s+t=1 or r=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge1();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    kPt = rkSeg.Origin()+rkSeg.Direction();
                    fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                    fR0 = 1.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0 )  // region 6p
                {
                    // min on face t=0 or s+t=1 or r=1
                    kTriSeg.Origin() = rkTri.Origin();
                    kTriSeg.Direction() = rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
                    fT = 0.0;
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
                    fS0 = 1.0-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    kPt = rkSeg.Origin()+rkSeg.Direction();
                    fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                    fR0 = 1.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1p
                {
                    // min on face s+t=1 or r=1
                    kTriSeg.Origin() = rkTri.Origin()+rkTri.Edge0();
                    kTriSeg.Direction() = rkTri.Edge1()-rkTri.Edge0();
                    fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fT);
                    fS = 1.0-fT;
                    kPt = rkSeg.Origin()+rkSeg.Direction();
                    fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
                    fR0 = 1.0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
            }
        }
    }
    else
    {
        // segment and triangle are parallel
        kTriSeg.Origin() = rkTri.Origin();
        kTriSeg.Direction() = rkTri.Edge0();
        fSqrDist = MgcSqrDistance(rkSeg,kTriSeg,&fR,&fS);
        fT = 0.0;

        kTriSeg.Direction() = rkTri.Edge1();
        fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
        fS0 = 0.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        kTriSeg.Origin() = rkTri.Origin() + rkTri.Edge0();
        kTriSeg.Direction() = rkTri.Edge1() - rkTri.Edge0();
        fSqrDist0 = MgcSqrDistance(rkSeg,kTriSeg,&fR0,&fT0);
        fS0 = 1.0-fT0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        fSqrDist0 = MgcSqrDistance(rkSeg.Origin(),rkTri,&fS0,&fT0);
        fR0 = 0.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        kPt = rkSeg.Origin()+rkSeg.Direction();
        fSqrDist0 = MgcSqrDistance(kPt,rkTri,&fS0,&fT0);
        fR0 = 1.0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }
    }

    if ( pfSegP )
        *pfSegP = fR;

    if ( pfTriP0 )
        *pfTriP0 = fS;

    if ( pfTriP1 )
        *pfTriP1 = fT;

    return MgcMath::Abs(fSqrDist);
}
//----------------------------------------------------------------------------
MgcReal MgcDistance (const MgcLine3& rkLine, const MgcTriangle3& rkTri,
    MgcReal* pfLinP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    return MgcMath::Sqrt(MgcSqrDistance(rkLine,rkTri,pfLinP,pfTriP0,
        pfTriP1));
}
//----------------------------------------------------------------------------
MgcReal MgcDistance (const MgcRay3& rkRay, const MgcTriangle3& rkTri,
    MgcReal* pfRayP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    return MgcMath::Sqrt(MgcSqrDistance(rkRay,rkTri,pfRayP,pfTriP0,
        pfTriP1));
}
//----------------------------------------------------------------------------
MgcReal MgcDistance (const MgcSegment3& rkSeg, const MgcTriangle3& rkTri,
    MgcReal* pfSegP, MgcReal* pfTriP0, MgcReal* pfTriP1)
{
    return MgcMath::Sqrt(MgcSqrDistance(rkSeg,rkTri,pfSegP,pfTriP0,
        pfTriP1));
}
//----------------------------------------------------------------------------
