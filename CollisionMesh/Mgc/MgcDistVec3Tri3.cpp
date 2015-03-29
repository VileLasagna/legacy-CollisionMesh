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

#include "MgcDistVec3Tri3.h"

//----------------------------------------------------------------------------
MgcReal MgcSqrDistance (const MgcVector3& rkPoint, const MgcTriangle3& rkTri,
    MgcReal* pfSParam, MgcReal* pfTParam)
{
    MgcVector3 kDiff = rkTri.Origin() - rkPoint;
    MgcReal fA00 = rkTri.Edge0().SquaredLength();
    MgcReal fA01 = rkTri.Edge0().Dot(rkTri.Edge1());
    MgcReal fA11 = rkTri.Edge1().SquaredLength();
    MgcReal fB0 = kDiff.Dot(rkTri.Edge0());
    MgcReal fB1 = kDiff.Dot(rkTri.Edge1());
    MgcReal fC = kDiff.SquaredLength();
    MgcReal fDet = MgcMath::Abs(fA00*fA11-fA01*fA01);
    MgcReal fS = fA01*fB1-fA11*fB0;
    MgcReal fT = fA01*fB0-fA00*fB1;
    MgcReal fSqrDist;

    if ( fS + fT <= fDet )
    {
        if ( fS < 0.0 )
        {
            if ( fT < 0.0 )  // region 4
            {
                if ( fB0 < 0.0 )
                {
                    fT = 0.0;
                    if ( -fB0 >= fA00 )
                    {
                        fS = 1.0;
                        fSqrDist = fA00+2.0*fB0+fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDist = fB0*fS+fC;
                    }
                }
                else
                {
                    fS = 0.0;
                    if ( fB1 >= 0.0 )
                    {
                        fT = 0.0;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = 1.0;
                        fSqrDist = fA11+2.0*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS = 0.0;
                if ( fB1 >= 0.0 )
                {
                    fT = 0.0;
                    fSqrDist = fC;
                }
                else if ( -fB1 >= fA11 )
                {
                    fT = 1;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < 0.0 )  // region 5
        {
            fT = 0.0;
            if ( fB0 >= 0.0 )
            {
                fS = 0.0;
                fSqrDist = fC;
            }
            else if ( -fB0 >= fA00 )
            {
                fS = 1.0;
                fSqrDist = fA00+2.0*fB0+fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            MgcReal fInvDet = 1.0/fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
        }
    }
    else
    {
        MgcReal fTmp0, fTmp1, fNumer, fDenom;

        if ( fS < 0.0 )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = 1.0;
                    fT = 0.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
            else
            {
                fS = 0.0;
                if ( fTmp1 <= 0.0 )
                {
                    fT = 1.0;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else if ( fB1 >= 0.0 )
                {
                    fT = 0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < 0.0 )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fT = 1.0;
                    fS = 0.0;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = 1.0 - fT;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
            else
            {
                fT = 0.0;
                if ( fTmp1 <= 0.0 )
                {
                    fS = 1.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else if ( fB0 >= 0.0 )
                {
                    fS = 0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if ( fNumer <= 0.0 )
            {
                fS = 0.0;
                fT = 1.0;
                fSqrDist = fA11+2.0*fB1+fC;
            }
            else
            {
                fDenom = fA00-2.0*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = 1.0;
                    fT = 0.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
        }
    }

    if ( pfSParam )
        *pfSParam = fS;

    if ( pfTParam )
        *pfTParam = fT;

    return MgcMath::Abs(fSqrDist);
}
//----------------------------------------------------------------------------
MgcReal MgcDistance (const MgcVector3& rkPoint, const MgcTriangle3& rkTri,
    MgcReal* pfSParam, MgcReal* pfTParam)
{
    return MgcMath::Sqrt(MgcSqrDistance(rkPoint,rkTri,pfSParam,pfTParam));
}
//----------------------------------------------------------------------------
