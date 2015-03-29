#include "MakeOctree.h"
#include <assert.h>

static const int TOO_MANY = 10;
static const int MAX_RECURSION_DEPTH = 7;

Octree* MakeOctree(
    const Tris& tris, const BBox& box, int recursionDepth)
{
    assert(!tris.empty());

    if (recursionDepth >= MAX_RECURSION_DEPTH)
    {
        // We cannot create child nodes, so we have to create a leaf node
        return new OctreeLeaf(tris, box);        
    }
        
    if (tris.size() < TOO_MANY)
    {
        // Number of tris is within limit. Create leaf node
        // containing all the tris.
        return new OctreeLeaf(tris, box);        
    }

    OctreeComposite* pComp = new OctreeComposite(box);

    // Create up to 8 children.
    // For each possible child node, create it if any tris fit in
    // the box. 

    for (int i = 0; i < 8; i++)
    {
        BBox childBox = box.MakeOctChild(i);

        // Check each triangle. 
        // If any vertex of the current tri is in the child box,
        //  create this octree node.
        Tris trisInChildBox;
        for (unsigned int j = 0; j < tris.size(); j++)
        {
            const Tri& tri = tris[j];

            // Is this triangle in the box ?
            // We test the centre point of the triangle. To get the centre point
            //  we get the average value for x, y, and z.
            Vec3f v;
            for (int k = 0; k < 3; k++)
            {
                v += tri.m_verts[k].m_point;
            }
            v *= (1.0/3.0); // v is centre of triangle
            if (childBox.Contains(v))
            {
                // Triangle centre is in this box, so add it to vector
                trisInChildBox.push_back(tri);
            }
        }

        if (!trisInChildBox.empty())
        {
            Octree* pChild = MakeOctree(
                trisInChildBox, childBox, recursionDepth + 1);

            pComp->AddChild(pChild);
        }
    }

    return pComp;
}

