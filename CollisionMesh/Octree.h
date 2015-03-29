#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include "Tri.h"
#include "BBox.h"
#include "Capsule.h"

class OctreeLeaf;

typedef std::vector<OctreeLeaf*> Leaves;
class Octree
{
public:
    virtual ~Octree() {}
    virtual void Draw() = 0;
	virtual void getIntersectingLeaves( const Capsule& capsule, Leaves* pLeaves, std::vector<Tri*>* result) = 0;
	virtual BBox getBox() {return m_box;}
	virtual void highlight(bool b = true){ if (b){colour[0] = 1; colour[1] = 0; colour[2] = 0;} else {colour[0] = 1; colour[1] = 1; colour[2] = 1;}}


protected:
    // Every concrete Octree type needs a bounding box
    BBox m_box;
	float colour[3];
};

class OctreeLeaf : public Octree
{
public:
    OctreeLeaf(const Tris& tris, const BBox& box);

	virtual void getIntersectingLeaves( const Capsule& capsule, Leaves* pLeaves, std::vector<Tri*>* result);

    virtual void Draw();

private:
    Tris m_tris;
};

class OctreeComposite : public Octree
{
public:
    OctreeComposite(const BBox& box);
	virtual void getIntersectingLeaves( const Capsule& capsule, Leaves* pLeaves, std::vector<Tri*>* result);
	virtual void highlight(bool b = true);

    virtual void Draw();
    void AddChild(Octree*);

	~OctreeComposite()
	{
		for(unsigned int i = 0; i < children.size(); i++)
		{
			delete children[i];
		}
	}

private:
    std::vector<Octree*> children;
};

#endif
