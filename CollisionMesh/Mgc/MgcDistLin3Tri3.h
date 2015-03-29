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

#ifndef MGCDISTLIN3TRI3_H
#define MGCDISTLIN3TRI3_H

#include "MgcLine3.h"
#include "MgcRay3.h"
#include "MgcSegment3.h"
#include "MgcTriangle3.h"

// squared distance measurements

MgcReal MgcSqrDistance (const MgcLine3& rkLine, const MgcTriangle3& rkTri,
    MgcReal* pfLinP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);

MgcReal MgcSqrDistance (const MgcRay3& rkRay, const MgcTriangle3& rkTri,
    MgcReal* pfRayP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);

MgcReal MgcSqrDistance (const MgcSegment3& rkSeg, const MgcTriangle3& rkTri,
    MgcReal* pfSegP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);


// distance measurements

MgcReal MgcDistance (const MgcLine3& rkLine, const MgcTriangle3& rkTri,
    MgcReal* pfLinP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);

MgcReal MgcDistance (const MgcRay3& rkRay, const MgcTriangle3& rkTri,
    MgcReal* pfRayP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);

MgcReal MgcDistance (const MgcSegment3& rkSeg, const MgcTriangle3& rkTri,
    MgcReal* pfSegP = 0, MgcReal* pfTriP0 = 0, MgcReal* pfTriP1 = 0);

#endif
