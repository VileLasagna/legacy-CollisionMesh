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

#ifndef MGCTRIANGLE3_H
#define MGCTRIANGLE3_H

#include "MgcVector3.h"


class MgcTriangle3
{
public:
    // Triangle points are tri(s,t) = b+s*e0+t*e1 where 0 <= s <= 1,
    // 0 <= t <= 1, and 0 <= s+t <= 1.

    MgcTriangle3 ();

    MgcVector3& Origin ();
    const MgcVector3& Origin () const;

    MgcVector3& Edge0 ();
    const MgcVector3& Edge0 () const;

    MgcVector3& Edge1 ();
    const MgcVector3& Edge1 () const;

protected:
    MgcVector3 m_kOrigin;
    MgcVector3 m_kEdge0;
    MgcVector3 m_kEdge1;
};

#include "MgcTriangle3.inl"

#endif
