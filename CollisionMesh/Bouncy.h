#ifndef BOUNCY_H
#define BOUNCY_H

#include "Vec3.h"
#include "CollisionMesh.h"
#include "Octree.h"

class Bouncy
{
public:
    void SetPos(const Vec3f& v);
    void SetVel(const Vec3f& v);

    void Update(Octree* tree);
	void Collides(Bouncy* other);
    void Draw();

private:

	float CalcTriLineSegSquareDist(const Tri& tri, const LineSeg& line);

    Vec3f m_pos;
    Vec3f m_oldpos; // to show capsule
    Vec3f m_vel;
	Vec3f m_colour;
};

#endif
