#include "Bouncy.h"
#include "Mgc/MgcPlane.h"
#include "Tri.h"
#include "Mgc/MgcDistLin3Tri3.h"
#include <gl/glut.h>

static const float RADIUS = 10.0f;
static const float GRAVITY = 20.0f;

void Bouncy::SetPos(const Vec3f& v) 
{ 
    m_pos = v; 
    m_oldpos = v;
}

void Bouncy::SetVel(const Vec3f& v) 
{ 
    m_vel = v; 
}

void Bouncy::Update(Octree* Tree)
{
	
    m_oldpos = m_pos; // remember old pos

    float dt = 0.1f; // TODO TEMP TEST need DT

    float y = m_vel.Y();
    y -= dt * GRAVITY; 
    

    // Get new pos, then check against terrain 
    Vec3f newPos = m_pos + (Vec3f(m_vel.X(), y, m_vel.Z()) * dt);


    int safety = 0; // if we loop this many times, bail out

    // Get any colliding triangles. For each one, move away from the triangle.
	std::vector<Tri*> Candidates;
	// Create capsule from line seg (old position, new position)
	Capsule collision(LineSeg(m_oldpos, newPos), RADIUS);
	Leaves HL;
	Tree->getIntersectingLeaves(collision, &HL, &Candidates); //Oh... yeah, the vector of leaves... kinda messed it up now T.T
	std::vector<std::pair<float,Tri*> > sdts;
	float rsq = RADIUS*RADIUS;

	while (true && (!Candidates.empty()) )
    {
        if (safety++ > 10) 
        {
            // Couldn't de-intersect. Bail out - do not set new position.
            return;
        }

		Capsule C(LineSeg(m_oldpos, newPos), RADIUS);
		sdts.clear();
		for (unsigned int i = 0; i < Candidates.size(); i++) //Narrow Phase
		{
			
			float dsq = CalcTriLineSegSquareDist((*Candidates[i]), C.GetLineSeg());
			if (dsq < rsq)
			{
				sdts.push_back(std::pair<float,Tri*>(dsq,Candidates[i]));
			}        
		}
		if(sdts.empty()) //no collision
		{
			break;
		}

        for (unsigned int i = 0; i < sdts.size(); i++)
        {
            const Tri& tri = *(sdts[i].second);
            // Move away from tri
            // Create plane from three verts
            // Move newPos in direction of normal
            // Make plane from 3 points
            MgcPlane plane(
                MgcVector3(tri.m_verts[0].X(), tri.m_verts[0].Y(), tri.m_verts[0].Z()), 
                MgcVector3(tri.m_verts[1].X(), tri.m_verts[1].Y(), tri.m_verts[1].Z()), 
                MgcVector3(tri.m_verts[2].X(), tri.m_verts[2].Y(), tri.m_verts[2].Z()));
            // NB order is important so normal faces in the dir we expect
            // (Again assuming it is normalized.)
            Vec3f norm(plane.Normal().x, plane.Normal().y, plane.Normal().z); 

            // Very simple collision response: move away from triangle
            // in direction of normal
            newPos += norm * 0.1f; 
        }
    }

    // Work out new velocity. We moved from m_pos to newPos in time dt.
     Vec3f newVel = (newPos - m_pos)  * (1.0f / dt);
    // This makes a big difference to the behaviour. 
    // Un/comment this line to see the difference.
    // If we do this, we change the velocity, i.e. the direction is influenced
    //  by the collision. If we don't do it, we try to plough forward in the
    //  original direction (e.g. like a player walking into a wall) 
    m_vel = newVel;

    m_pos = newPos;
}

void Bouncy::Draw()
{
    glDisable(GL_LIGHTING);

	glColor3f(m_colour.X(),m_colour.Y(),m_colour.Z());
    glPushMatrix();
    glTranslatef(m_oldpos.X(), m_oldpos.Y(), m_oldpos.Z());
    glutWireSphere(RADIUS, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_pos.X(), m_pos.Y(), m_pos.Z());
    glutWireSphere(RADIUS, 10, 10);
    glPopMatrix();
	glColor3f(1,1,1);

    glEnable(GL_LIGHTING);
}

void Bouncy::Collides(Bouncy* other)
{
	Capsule C(LineSeg(m_pos,m_pos+((m_vel+ other->m_vel)*0.1f)), RADIUS);
	Sphere S(other->m_pos,RADIUS);
	if(!C.Collides(S))
	{
		m_colour = Vec3f(1,1,1);
		other->m_colour = Vec3f(1,1,1);
		return;
	}
		//Vec3f P0 = m_pos;
		//Vec3f otherP0 = other->m_pos;
		//Vec3f direc = P0 - otherP0;
		//direc.Normalize();
		//direc *= 0.1;
		//while (C.Collides(S))
		//{
		//	other->m_pos+=direc;
		//	S.setPos(other->m_pos);

		//}
		m_colour = Vec3f(0,1,0);
		other->m_colour = Vec3f(0,1,0);
		Vec3f PoI = (m_pos+other->m_pos)/2;//Rough estimation and only "works" because they're the same RADIUS
		Vec3f N = m_vel+ other->m_vel;
		float VL = m_vel.Length();
		m_vel += N;
		m_vel.Normalize();
		m_vel *= VL;
		VL = other->m_vel.Length();
		other->m_vel -= N;
		other->m_vel.Normalize();
		other->m_vel *= VL;




}


float Bouncy::CalcTriLineSegSquareDist(
    const Tri& tri, const LineSeg& line)
{
    MgcSegment3 mgcSeg;
    mgcSeg.Origin() = MgcVector3(line.GetStart().X(), line.GetStart().Y(), line.GetStart().Z());
    Vec3f dir = line.GetEnd() - line.GetStart();
    mgcSeg.Direction() = MgcVector3(dir.X(), dir.Y(), dir.Z());

    MgcTriangle3 mgcTri;
    mgcTri.Origin() = MgcVector3(tri.m_verts[0].X(), tri.m_verts[0].Y(), tri.m_verts[0].Z());
    Vec3f edge0 = tri.m_verts[1] - tri.m_verts[0];
    Vec3f edge1 = tri.m_verts[2] - tri.m_verts[0];
    mgcTri.Edge0() = MgcVector3(edge0.X(), edge0.Y(), edge0.Z());
    mgcTri.Edge1() = MgcVector3(edge1.X(), edge1.Y(), edge1.Z());

    return MgcSqrDistance(mgcSeg, mgcTri);
}