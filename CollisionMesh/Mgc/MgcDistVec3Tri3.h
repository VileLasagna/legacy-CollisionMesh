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

#ifndef MGCDISTVEC3TRI3_H
#define MGCDISTVEC3TRI3_H

#include "MgcTriangle3.h"

// squared distance measurements

MgcReal MgcSqrDistance (const MgcVector3& rkPoint, const MgcTriangle3& rkTri,
    MgcReal* pfSParam = 0, MgcReal* pfTParam = 0);


// distance measurements

MgcReal MgcDistance (const MgcVector3& rkPoint, const MgcTriangle3& rkTri,
    MgcReal* pfSParam = 0, MgcReal* pfTParam = 0);

#endif
