#ifndef TERRAIN_H
#define TERRAIN_H

#include "CollisionMesh.h"
#include "SDLBitmap.h"

class Terrain
{
public:
    bool LoadHeightmap(const std::string& filename);
    void MakeMesh(CollisionMesh* pMesh, int numSquares, float squareSideLength);

protected:
    void MakeTerrain(CollisionMesh::Tris* pTris, int numSquares, float squareSideLength);
    float GetY(int x, int y);

private:
    SDLBitmap m_bitmap;
};

#endif
