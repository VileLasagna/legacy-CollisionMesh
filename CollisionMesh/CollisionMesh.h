#ifndef COLLISION_MESH_H
#define COLLISION_MESH_H

#include <vector>
#include "Vec3.h"
#include "Tri.h"
#include "Capsule.h"

class CollisionMesh
{
public:

    typedef std::vector<Tri> Tris;

    void AddTri(const Tri& t);
    void Draw(); // for debugging

    typedef std::pair<float, const Tri*> SqDistTri;
    typedef std::vector<SqDistTri> SqDistTris;

    // Returns true if the given capsule intersects this mesh.
    // If there is an intersection, the vector is populated:
    // each element is a pair. The first member is the squared
    // distance from the capsule line seg to the Tri.
    // The second member is a pointer to the Tri.
    bool Intersects(const Capsule& c, SqDistTris* pVec);
	 Tris& getTris() { return m_tris;}

private:
    bool CapsuleTriIntersection(const Capsule& c, const Tri& t);

private:
    Tris m_tris;
};

#endif
