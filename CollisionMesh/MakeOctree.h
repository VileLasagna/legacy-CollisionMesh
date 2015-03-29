#ifndef MAKE_OCTREE_H
#define MAKE_OCTREE_H

#include "Octree.h"

Octree* MakeOctree(const Tris& tris, const BBox& box, int recursionDepth);

#endif
