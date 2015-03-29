#ifndef TRI_H
#define TRI_H

#include "Vertex.h"

struct Tri
{
	void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			m_verts[i].Draw();
		}
	}

	Vertex m_verts[3];
};

typedef std::vector<Tri> Tris;

#endif
