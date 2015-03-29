#include "CollisionMesh.h"
#include "Mgc/MgcDistLin3Tri3.h"
#include <windows.h>
#include <GL/gl.h>

void CollisionMesh::AddTri(const Tri& t)
{
    m_tris.push_back(t);
}

void CollisionMesh::Draw()
{
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < m_tris.size(); i++)
    {
        const Tri& tri = m_tris[i];
        for (int j = 0; j < 3; j++)
        {
            glVertex3f(tri.m_verts[j].X(), tri.m_verts[j].Y(), tri.m_verts[j].Z());
        }
    }   
    glEnd();
}
//
//float CalcTriLineSegSquareDist(
//    const Tri& tri, const LineSeg& line)
//{
//    MgcSegment3 mgcSeg;
//    mgcSeg.Origin() = MgcVector3(line.GetStart().X(), line.GetStart().Y(), line.GetStart().Z());
//    Vec3f dir = line.GetEnd() - line.GetStart();
//    mgcSeg.Direction() = MgcVector3(dir.X(), dir.Y(), dir.Z());
//
//    MgcTriangle3 mgcTri;
//    mgcTri.Origin() = MgcVector3(tri.m_verts[0].X(), tri.m_verts[0].Y(), tri.m_verts[0].Z());
//    Vec3f edge0 = tri.m_verts[1] - tri.m_verts[0];
//    Vec3f edge1 = tri.m_verts[2] - tri.m_verts[0];
//    mgcTri.Edge0() = MgcVector3(edge0.X(), edge0.Y(), edge0.Z());
//    mgcTri.Edge1() = MgcVector3(edge1.X(), edge1.Y(), edge1.Z());
//
//    return MgcSqrDistance(mgcSeg, mgcTri);
//}

bool CollisionMesh::Intersects(const Capsule& c, SqDistTris* pVec)
{
    // NB This is slow, we check every tri. We don't bail as soon as we
    // find a colliding triangle.

    pVec->clear();

    float rsq = c.GetRadius();
    rsq *= rsq;
    // Check each tri - if capsule intersects tri, add to pTris 
    for (unsigned int i = 0; i < m_tris.size(); i++)
    {
        const Tri& tri = m_tris[i];
	   float dsq = 0; //CalcTriLineSegSquareDist(tri, c.GetLineSeg());
        if (dsq < rsq)
        {
            SqDistTri sdt(dsq, &tri);
            pVec->push_back(sdt);
        }        
    }

    return !pVec->empty();
}

